//
// Created by Adil Gadi on 4/6/25.
//

#ifndef SERVER_H
#define SERVER_H
#include <string>
#include <mutex>
#include "cstring"
#include "dequeue.h"

constexpr static int SERVER_PORT = 8080;
constexpr static int BUFFER_SIZE = 1024;
static auto TOKEN_HEADER = "\r\nRedis-Token";
const static size_t TOKEN_HEADER_LEN = strlen(TOKEN_HEADER);
constexpr static int MAX_THREADS = 100;
constexpr static int TOKEN_LEN = 8;

inline Dequeue<int> tasks;

inline std::mutex tasks_mtx;
inline std::condition_variable cv;
inline int server_socket;
inline int working = 0;

struct HandlerResponse {
    char body[1024];
    int status;
};

typedef void (*server_handler) (const char*, const char*, HandlerResponse*);

void init_server(server_handler);
void worker(int id, server_handler);
void handle_request(int client, server_handler);

#endif //SERVER_H
