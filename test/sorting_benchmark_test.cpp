#include "catch.hpp"

#include "sorting_benchmark/sorting_benchmark.hpp"
#include "sorting_benchmark/utility.hpp"

#include <utility>

using namespace srtbch;

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

TEST_CASE("Sort Array", "[sort]") {
  SortBench<int, BubbleSort, Generator> bench {true, true};
  std::size_t size = GENERATE(3, 10, 50, 100, 1000);
  std::size_t measure_num = GENERATE(1, 5, 10);
  bench(size, measure_num);
  for (auto arr : bench.notsorted_arrays()) {
    CHECK_NOFAIL(std::is_sorted(std::begin(arr),std::end(arr)) == false);
  }
  for (auto arr : bench.sorted_arrays()) {
    REQUIRE(std::is_sorted(std::begin(arr),std::end(arr)) == true);
  }
}

TEST_CASE() {}