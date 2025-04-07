//
// Created by Adil Gadi on 4/6/25.
//
#include <iostream>
#include <thread>
#include <mutex>

#include "sys/socket.h"
#include "netinet/in.h"
#include "unistd.h"
#include "cstdlib"
#include "cstdio"

#include "server.h"

#define LOG(x) std::cout << x << std::endl

void init_server(int (*handler) (const char*, const char*)) {
    int s = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    int success = bind(s, (sockaddr *) &addr, sizeof addr);

    if (success < 0) {
        LOG("Unable to bind");
        close(s);
    }

    //Initializes workers

    std::thread workers[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        workers[i] = std::thread(worker, i, handler);
    };

    listen(s, 10);

    LOG("Listening on PORT: " << SERVER_PORT);

    while (true) {
        int client = accept(s, 0, 0);

        if (client == -1) {
            continue;
        }

        mtx.lock();
        // LOG("Working: " << working);

        if (working > 5) {
            LOG("Utilization: " << working << " / " << MAX_THREADS);

            if (working >= MAX_THREADS) {
                close(client);
                LOG("Server is handling too many requests");
                mtx.unlock();
                continue;
            }
        }

        tasks.push(client);
        mtx.unlock();

        cv.notify_one();
        // LOG("Notified a Worker");
    }

    close(s);
    LOG("Closed successfully server");
}

void worker(int id, int (*handler) (const char*, const char*)) {
    int client = -1;
    while (true) {
        {
            std::unique_lock lock(mtx);
            cv.wait(lock, [&client]() {
                DequeueItem<int>* head = tasks.getHeadItem();

                if (head == nullptr) {
                    return false;
                }

                client = head->data;
                tasks.unshift();
                working++;

                return true;
            });
        }

        // LOG("Received");
        // LOG("From: " << id);
        if (client == -1) {
            continue;
        }
        handle_request(client, handler);
        mtx.lock();
        working--;
        mtx.unlock();
    }
}

void handle_request(int client,int (*handler) (const char*, const char*)) {
        char request[BUFFER_SIZE] = {0};
        auto request_size = recv(client, &request, BUFFER_SIZE - 1, 0);
        request[request_size] = 0;

        if (request_size < 0) {
            // LOG("Recv Failed");
            return;
        }

        char* token = nullptr;
        char* body = nullptr;
        for (int i = 0; i < BUFFER_SIZE; i++) {
            const char c = request[i];
            if (c == '\r') {
                if (strncmp(&request[i], TOKEN_HEADER, TOKEN_HEADER_LEN) == 0) {

                    if (i + TOKEN_HEADER_LEN + 2 + TOKEN_LEN < request_size) {
                        token = &request[i] + TOKEN_HEADER_LEN + 2;
                        LOG(token[0] << token[1] << token[2]);
                    }
                } else if (strncmp(&request[i], "\r\n\r\n", 4) == 0) {
                    body = &request[i] + 4;
                }

                continue;
            }
            if (c == 0) {
                break;
            }
            // std::cout << c;
        }

        if (token == nullptr || body == nullptr) {
            return;
        }

        handler(token, body);

        char response[100];
        snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n\"Hello\"");
        send(client, &response, strlen(response), 0);

        close(client);
        // LOG("Closed client");
}
