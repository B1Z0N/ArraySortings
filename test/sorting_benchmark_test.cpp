#include "catch.hpp"

#include "sorting_benchmark/sorting_benchmark.hpp"
#include "sorting_benchmark/utility.hpp"

#include <utility>

template <typename T>
class BubbleSort {
 public:
  void operator()(T* arr, std::size_t size) {
    for (std::size_t i = 0; i < size - 1; ++i) {
      for (std::size_t j = i + 1; j < size; ++j) {
        if (arr[i] > arr[j]) {
          std::swap(arr[i], arr[j]);
        }
      }
    }
  }
};

TEST_CASE() {}

TEST_CASE() {}