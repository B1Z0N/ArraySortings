cmake_minimum_required(VERSION 3.15.1)
project(SortingBenchmark)

include_directories(include)
include_directories(lib/catch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -pedantic-errors")

set(TEST_SOURCES
  test/array_element_test.cpp
  # test/sort_bench_test.cpp
  test/main.cpp
  )

add_executable(test_runner ${TEST_SOURCES})
