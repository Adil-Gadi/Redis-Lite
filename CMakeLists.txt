cmake_minimum_required(VERSION 3.30)
project(Redis_Lite)

set(CMAKE_CXX_STANDARD 23)

add_executable(Redis_Lite main.cpp
        HashMap.cpp
        HashMap.h)

add_subdirectory(external/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)

target_link_libraries(Redis_Lite gtest gtest_main)

add_executable(run_tests tests/test_runner.cpp tests/sample_test.cpp
        HashMap.cpp
        HashMap.h)

target_link_libraries(run_tests gtest gtest_main)
