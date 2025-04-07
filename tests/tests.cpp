//
// Created by Adil Gadi on 3/30/25.
//
#include <gtest/gtest.h>
#include "../HashMap.h"

TEST(HashMapTest, Add) {
    add_item("hello", "world");

    const char* result = get_item("hello");

    EXPECT_EQ(strcmp(result, "world"), 0);
}

TEST(HashMapTest, Delete) {
    add_item("hello", "world");

    bool deleted = delete_item("hello");

    const char* result = get_item("hello");

    EXPECT_EQ(deleted, true);
    EXPECT_EQ(result, nullptr);
}

TEST(HashMapTest, AddSpecialCase) {
    add_item("req", "HTTP/1.0 200 OK\r\n\r\nHello");

    const char* result = get_item("req");

    EXPECT_EQ(strcmp(result, "HTTP/1.0 200 OK\r\n\r\nHello"), 0);
}

TEST(HashMapTest, InitMap) {
    const char* result = get_item("here");

    EXPECT_EQ(result, nullptr);
}

TEST(HashMapTest, Flush) {
    add_item("hello", "world");

    flush();

    const char* result = get_item("hello");

    EXPECT_EQ(result, nullptr);
}
