cmake_minimum_required(VERSION 3.15.1)
project(SortingBenchmark)

include_directories(include)
include_directories(lib/catch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -pedantic-errors")

set(TEST_SOURCES
  test/array_element_test.cpp
  test/utility_test.cpp
  test/sorting_benchmark_test.cpp
  test/sortings_test.cpp
  test/main.cpp
  )

add_executable(test_runner ${TEST_SOURCES})

# generate static library from the sources
add_library(srtbch STATIC "src/main.cpp")

# set the location for library installation -- current folder in this case
# not really necessary in this example. Use "sudo make install" to apply
install(TARGETS srtbch DESTINATION .)