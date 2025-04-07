//
// Created by Adil Gadi on 4/6/25.
//

#ifndef SERVER_H
#define SERVER_H
#include <cstring>
#include <mutex>

#include "dequeue.h"

constexpr static int SERVER_PORT = 8080;
constexpr static int BUFFER_SIZE = 1024;
const static char* TOKEN_HEADER = "\r\nRedis-Token";
const static size_t TOKEN_HEADER_LEN = strlen(TOKEN_HEADER);
constexpr static int MAX_THREADS = 100;
constexpr static int TOKEN_LEN = 8;

inline Dequeue<int> tasks;

inline std::mutex mtx;
inline std::condition_variable cv;
inline int working = 0;

void init_server(int (*handler) (const char*, const char*));
void worker(int id, int (*handler) (const char*, const char*));
void handle_request(int client, int (*handler) (const char*, const char*));

#endif //SERVER_H
