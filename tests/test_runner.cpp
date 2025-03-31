//
// Created by Adil Gadi on 3/30/25.
//
#include <gtest/gtest.h>
#include "../HashMap.h"

int main(int argc, char **argv) {
    init_map();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}