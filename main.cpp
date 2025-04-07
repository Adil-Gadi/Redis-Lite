#include <iostream>
#include <string>

#include "HashMap.h"
#include "server.h"
#define LOG(x) std::cout << x << std::endl

void extract_params(const std::string &input, int result[3]) {
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

int handler(const char* auth, const char* body) {

    std::string_view token(auth);
    const std::string input(body);

    LOG("Token: " << token.substr(0, 8));
    LOG("Input: " << input);
    // std::cout << "> ";
    // std::getline(std::cin, input);

    // if (input.starts_with("EXIT")) {
    //     break;
    // }

    int result[3];
    extract_params(input, result);
    if (result[0] == -1 || result[1] == -1) {
        LOG("No key provided");
        // continue;
        return -1;
    }

    std::string key = input.substr(result[0], result[1] - result[0] + 1);
    std::string value = result[2] == -1 ? "" : input.substr(result[2]);
    // std::println("{} = {}", key, value);
    if (input.starts_with("ADD")) {
        const bool success = add_item(key.c_str(), value.c_str());

        if (!success) {
            LOG("Error: unsuccessful at adding item");

            return -1;
            // continue;
        }

        if (result[2] == -1) {
            LOG("Error: no associated value provided");
            return -1;
            // continue;
        }

        return 0;
        // continue;
    }

    if (input.starts_with("GET")) {
        const char *value = get_item(key.c_str());

        if (value == nullptr) {
            LOG("Error: unable to find value with specified key");
            // continue;
            return -1;
        }

        LOG(value);
        // continue;
        return 0;
    }

    if (input.starts_with("DELETE") || input.starts_with("REMOVE")) {
        bool success = delete_item(key.c_str());

        if (!success) {
            LOG("Error: unable to delete item");
            // continue;
            return -1;
        }

        return 0;
    }

    if (input.starts_with("FLUSH")) {
        flush();
    }

  return 0;
}

int main() {
    init_map();

    init_server(handler);

    flush();

    return 0;
}
