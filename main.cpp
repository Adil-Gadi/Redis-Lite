#include <iostream>
#include <string>
#include <print>
#include <sstream>
#define LOG(x) std::cout << x << std::endl

constexpr static int MAX = 100;
constexpr static int INIT_SIZE = 5;

typedef struct {
    int size = INIT_SIZE;
    int amount = 0;
    char **ptr = nullptr;
    int empty = 0;
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
            const char* val = src[i] + sizeof(key);
            return val;
        }
    }

    return nullptr;
}

bool delete_item(const char* key) {
    const int index = hash(key);

    HashArray *item = &map[index];
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
            item->empty++;
            return true;
        }
    }

    return false;
}

bool add_item(const char *key, const char *value) {
    const int index = hash(key);

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
        item->ptr = (char**)realloc((char**)item->ptr, new_size);
        item->size = new_size;
    }

    const char* found = get_item(key);

    if (found != nullptr) {
        delete_item(key);
    }

    char** src = &map[index].ptr[location];

    *src = (char *) malloc(strlen(value) + 1 + strlen(key) + 1);
    strcpy(*src, key);

    // strcat(*src, value);
    char* sub = src[0] + sizeof(key);

    strcpy(sub, value);
    item->amount++;
    return true;
}

bool flush() {
    for (int i = 0; i < MAX; i++) {
        // map[i].amount = 0;
        // map[i].ptr = nullptr;
        // map[i].size = INIT_SIZE;
        HashArray* array = &map[i];
        char** src = array->ptr;
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

void extract_params(const std::string& input) {

    int keyStart;
    int keyEnd;

    int valStart;
    int valEnd;

    for (int i = 0; i < input.length(); i++) {
        LOG(input.at(i));
        if (input.at(i) == '"') {
            std::println("{} {}", i, input.at(i));
        }
    }
}

int main() {
    init_map();

    while (true) {
        std::string input;
        std::getline(std::cin, input);

       // LOG(input);

        std::stringstream ss(input);

        extract_params(input);
        if (input.starts_with("ADD")) {
        }

        if (input.starts_with("GET")) {

        }

        if (input.starts_with("DELETE") || input.starts_with("REMOVE")) {

        }

        if (input.starts_with("EXIT")) {
            break;
        }
    }

    flush();

    return 0;
}
