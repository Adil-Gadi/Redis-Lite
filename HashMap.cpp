//
// Created by Adil Gadi on 3/30/25.
//

#include "cstdlib"
#include "cstring"
#include "server.h"

#include "HashMap.h"

void init_map() {
    for (int i = 0; i < MAX; i++) {
        map[i] = HashArray();
        map[i].ptr = (char **) malloc(sizeof(char **) * INIT_SIZE);
        map[i].size = INIT_SIZE;
    }
}

int hash(const char *key) {
    int sum = 0;
    const size_t len = strlen(key);
    for (int i = 0; i < len; i++) {
        int x = key[i];
        sum += x * 100;
    }

    return (sum / len) % MAX;
}

const char *get_item(const char *key) {
    const int index = hash(key);

    std::lock_guard lock(tasks_mtx);
    const HashArray *item = &map[index];
    const int amount = item->amount;
    char **src = item->ptr;

    if (src == nullptr) {
        return nullptr;
    }

    for (int i = 0; i < amount; i++) {
        if (src[i] == nullptr) {
            continue;
        }

        int len = strlen(key);
        if (len > strlen(src[i])) {
            continue;
        }

        if (strncmp(src[i], key, strlen(key)) == 0) {
            const char *val = src[i] + strlen(key) + 1;
            return val;
        }
    }

    return nullptr;
}

bool delete_item(const char *key) {
    const int index = hash(key);

    std::lock_guard lock(tasks_mtx);
    HashArray *item = &map[index];
    const int amount = item->amount;
    char **src = item->ptr;

    for (int i = 0; i < amount; i++) {
        if (src[i] == nullptr) {
            continue;
        }

        int len = strlen(key);

        if (len > strlen(src[i])) {
            continue;
        }

        if (strncmp(src[i], key, strlen(key)) == 0) {
            free(src[i]);
            src[i] = nullptr;
            item->empty++;
            return true;
        }
    }

    return false;
}

bool add_item(const char *key, const char *value) {
    const int index = hash(key);

    std::lock_guard lock(map_mtx);
    HashArray *item = &map[index];
    const int amount = item->amount;

    int location = amount;
    if (amount == item->size) {
        if (item->empty > 0) {
            for (int i = 0; i < amount; i++) {
                if (item->ptr[i] == nullptr) {
                    location = i;
                    item->empty--;
                    break;
                }
            }
        }

        //realloc
        size_t new_size = sizeof(item->ptr) * item->size * 2;
        item->ptr = (char **) realloc((char **) item->ptr, new_size);
        item->size = new_size;
    }

    // TODO: integrate into add fn
    const char *found = get_item(key);

    if (found != nullptr) {
        // TODO: integrate into add fn
        delete_item(key);
    }

    char **src = &map[index].ptr[location];

    *src = (char *) malloc(strlen(value) + 1 + strlen(key) + 1);
    strcpy(*src, key);

    // strcat(*src, value);
    char *sub = src[0] + strlen(key) + 1;

    strcpy(sub, value);

    item->amount++;
    return true;
}

bool delete_map() {
    std::lock_guard lock(tasks_mtx);
    for (int i = 0; i < MAX; i++) {
        // map[i].amount = 0;
        // map[i].ptr = nullptr;
        // map[i].size = INIT_SIZE;
        HashArray *array = &map[i];
        char **src = array->ptr;
        for (int j = 0; j < map[i].amount; j++) {
            if (src[j] == nullptr) continue;
            free(src[j]);
            src[j] = nullptr;
        }

        free(src);
        src = nullptr;
        array->amount = 0;
        array->size = 0;
    }

    return true;
}

void flush() {
    delete_map();
    init_map();
}