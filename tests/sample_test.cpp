//
// Created by Adil Gadi on 3/30/25.
//
#include <gtest/gtest.h>

// Function to be tested
int add(int a, int b) {
    return a + b;
}

// Test case
TEST(AdditionTest, HandlesPositiveNumbers) {
    EXPECT_EQ(add(1, 2), 3);
}