#include <iostream>
#include <string>
#include <print>
#define LOG(x) std::cout << x << std::endl

constexpr static int MAX = 100;
constexpr static int INIT_SIZE = 5;

typedef struct {
    int size = INIT_SIZE;
    int amount = 0;
    char **ptr = nullptr;
} HashArray;

HashArray map[MAX];

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

    const HashArray *item = &map[index];
    const int amount = item->amount;
    char** src = item->ptr;

    for (int i = 0; i < amount; i++) {

        if (src[i] == nullptr) {
            continue;
        }

        int len = strlen(key);
        if (len > strlen(src[i])) {
            continue;
        }

        if (strncmp(src[i], key, strlen(key)) == 0) {
            const char* val = src[i] + strlen(key);
            return val;
        }
    }

    return nullptr;
}

bool delete_item(const char* key) {
    const int index = hash(key);

    const HashArray *item = &map[index];
    const int amount = item->amount;
    char** src = item->ptr;

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
            return true;
        }
    }

    return false;
}

bool add_item(const char *key, const char *value) {
    const int index = hash(key);

    HashArray *item = &map[index];
    const int amount = item->amount;

    if (amount == item->size) {
        //realloc
    }

    const char* found = get_item(key);

    if (found != nullptr) {
        delete_item(key);
    }

    char** src = &map[index].ptr[amount];

    *src = (char *) malloc(sizeof(value) + sizeof(key));
    strcpy(*src, key);
    strcat(*src, value);

    item->amount++;
}

bool flush() {
    for (int i = 0; i < MAX; i++) {
        char** src = map[i].ptr;
        for (int j = 0; j < map[i].amount; j++) {
            if (src[j] == nullptr) continue;
            free(src[j]);
            src[j] = nullptr;
            LOG("yes");
        }
    }

    return true;
}

int main() {
    init_map();

    const char *key1 = "Sample";
    const char *key2 = "True";
    const char *key3 = "False";
    add_item(key1, "hello");
    add_item(key1, "bye");

    // LOG(get_item(key1));

    // LOG(delete_item(key1));
    // LOG(delete_item(key1));

    flush();
    // init_map();
    auto m = map[66].ptr[1];
    if (m == nullptr) {
        LOG(1);
    }
    LOG(get_item(key1));


    return 0;
}
