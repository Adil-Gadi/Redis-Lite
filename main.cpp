#include <iostream>
#include <string>
#include <print>
#include <sstream>
#define LOG(x) std::cout << x << std::endl

constexpr static int MAX = 1000;
constexpr static int INIT_SIZE = 5;

typedef struct HashArray {
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

bool flush() {
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
        keyEnd = input.length() - 1;

    int valueStart = -1;
    if (keyEnd != -1 && (keyEnd + 2) < input.length())
        valueStart = keyEnd + 2;

    result[0] = keyStart;
    result[1] = keyEnd;
    result[2] = valueStart;
}

int main() {
    init_map();

    while (true) {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.starts_with("EXIT")) {
            break;
        }

        int result[3];
        extract_params(input, result);
        if (result[0] == -1 || result[1] == -1) {
            LOG("No key provided");
            continue;
        }
        std::string key = input.substr(result[0], result[1] - result[0] + 1);
        std::string value = result[2] == -1 ? "" : input.substr(result[2]);
        // std::println("{} = {}", key, value);
        if (input.starts_with("ADD")) {
            const bool success = add_item(key.c_str(), value.c_str());

            if (!success) {
                LOG("Error: unsuccessful at adding item");
                continue;
            }

            if (result[2] == -1) {
                LOG("Error: no associated value provided");
            }

            continue;
        }

        if (input.starts_with("GET")) {
            const char *value = get_item(key.c_str());

            if (value == nullptr) {
                LOG("Error: unable to find value with specified key");
                continue;
            }

            LOG(value);
            continue;
        }

        if (input.starts_with("DELETE") || input.starts_with("REMOVE")) {
            bool success = delete_item(key.c_str());

            if (!success) {
                LOG("Error: unable to delete item");
                continue;
            }
        }
    }

    flush();

    return 0;
}
