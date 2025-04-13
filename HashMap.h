//
// Created by Adil Gadi on 3/30/25.
//

#ifndef HASHMAP_H
#define HASHMAP_H
#include <mutex>

constexpr static int MAX = 1000;
constexpr static int INIT_SIZE = 2;

typedef struct HashArray {
    int size = INIT_SIZE;
    int amount = 0;
    char **ptr = nullptr;
    int empty = 0;
} HashArray;

inline std::mutex map_mtx;
inline HashArray map[MAX];

void init_map();

int hash(const char* key);

const char *get_item(const char *key);

bool delete_item(const char *key);

bool add_item(const char *key, const char *value);

bool flush();

#endif //HASHMAP_H
