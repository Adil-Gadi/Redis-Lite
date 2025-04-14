#include <iostream>
#include <string>
#include <unistd.h>

#include "HashMap.h"
#include "server.h"
#define LOG(x) std::cout << x << std::endl
// #define LOG(x) ;
// static std::string_view authToken("12345678");
const char* authToken = "12345678";

void extract_params(const std::string_view &input, int result[3]) {
    int keyStart = -1;
    int keyEnd = -1;
    int spacesIn = 0;
    int spacesInBefore = 0;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ' ') {
            spacesInBefore = spacesIn;
            spacesIn++;
        } else continue;

        if (!(i + 1 < input.length())) {
            continue;
        }

        if (spacesIn == 1 && spacesInBefore == 0 && keyStart == -1) {
            keyStart = i + 1;
        } else if (spacesIn == 2 && spacesInBefore == 1 && keyEnd == -1) {
            keyEnd = i - 1;
        }
    }

    if (keyEnd == -1 && keyStart != -1)
        keyEnd = (int)input.length() - 1;

    int valueStart = -1;
    if (keyEnd != -1 && (keyEnd + 2) < input.length())
        valueStart = keyEnd + 2;

    result[0] = keyStart;
    result[1] = keyEnd;
    result[2] = valueStart;
}

void handler(const char* auth, const char* body, HandlerResponse* res) {

    const std::string_view token(auth);
    const std::string_view input(body);

    if (token.substr(0,8) != authToken) {
        res->status = 400;
        strcpy(res->body, "Error: Invalid Authentication Token");
        return;
    }

    // LOG(token.substr(0, 8).compare("12345678910"));
    // LOG("Token: " << token.substr(0, 8));
    // LOG("Input: " << input);
    // std::cout << "> ";
    // std::getline(std::cin, input);

    // if (input.starts_with("EXIT")) {
    //     break;
    // }

    int result[3];
    extract_params(input, result);
    if (result[0] == -1 || result[1] == -1) {

        if (input.starts_with("FLUSH")) {
            delete_map();
            init_map();
            res->status = 200;
            strcpy(res->body, "Success");
            return;
        }

        LOG("No key provided");
        res->status = 400;
        strcpy(res->body, "No key provided");

        // continue;
        return;
    }

    std::string key(input.substr(result[0], result[1] - result[0] + 1));
    std::string value(result[2] == -1 ? "" : input.substr(result[2]));
    // std::println("{} = {}", key, value);
    if (input.starts_with("ADD")) {
        const bool success = add_item(key.c_str(), value.c_str());

        if (!success) {
            // LOG("Error: unsuccessful at adding item");
            res->status = 500;
            strcpy(res->body, "Error: unsuccessful at adding item");

            return;
            // continue;
        }

        if (result[2] == -1) {
            res->status = 400;
            strcpy(res->body, "Error: no associated value provided");
            return;
            // continue;
        }

        res->status = 200;
        strcpy(res->body, "Success");

        return;
        // continue;
    }

    if (input.starts_with("GET")) {
        const char *value = get_item(key.c_str());

        if (value == nullptr) {
            // LOG("Error: unable to find value with specified key");
            res->status = 400;
            strcpy(res->body, "Error: unable to find value with specified key");
            // continue;
            return;
        }

        // LOG(value);

        res->status = 200;
        strcpy(res->body, value);
        // continue;
        return;
    }

    if (input.starts_with("DELETE") || input.starts_with("REMOVE")) {
        bool success = delete_item(key.c_str());

        if (!success) {
            // LOG("Error: unable to delete item");
            res->status = 500;
            strcpy(res->body, "Error: unable to delete item");
            // continue;
            return;
        }

        res->status = 200;
        strcpy(res->body, "Success");
    }

}

void close_server(int) {
    delete_map();
    close(server_socket);
    LOG("Closed server successfully");
    exit(0);
}

int main() {
    init_map();

    signal(SIGTERM, close_server);

    init_server(handler);

    return 0;
}
