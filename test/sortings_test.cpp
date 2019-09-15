#include "catch.hpp"

#include "sorting_benchmark/sorting_benchmark.hpp"
#include "sorting_benchmark/sortings.hpp"
#include "sorting_benchmark/utility.hpp"

#include <utility>

using namespace srtbch;
using namespace sortings;

TEST_CASE("Selection Sorting Array", "[sort]") {
  SortBench<int, SelectionSort, Generator> bench{true, true};
  std::size_t size = GENERATE(3, 10, 50, 100, 1000, 1000);
  std::size_t measure_num = GENERATE(1, 5, 10);

  bench(size, measure_num);

  for (auto arr : bench.sorted_arrays()) {
    REQUIRE(std::is_sorted(std::begin(arr), std::end(arr)) == true);
  }
}

TEST_CASE("Bubble Sorting Array", "[sort]") {
  SortBench<int, BubbleSort, Generator> bench{true, true};
  std::size_t size = GENERATE(3, 10, 50, 100, 1000, 1000);
  std::size_t measure_num = GENERATE(1, 5, 10);

  bench(size, measure_num);

  for (auto arr : bench.sorted_arrays()) {
    REQUIRE(std::is_sorted(std::begin(arr), std::end(arr)) == true);
  }
}


TEST_CASE("Insertion Sorting Array", "[sort]") {
  SortBench<int, InsertionSort, Generator> bench{true, true};
  std::size_t size = GENERATE(3, 10, 50, 100, 1000, 1000);
  std::size_t measure_num = GENERATE(1, 5, 10);

  bench(size, measure_num);

  for (auto arr : bench.sorted_arrays()) {
    REQUIRE(std::is_sorted(std::begin(arr), std::end(arr)) == true);
  }
}

// TEST_CASE("Merge Sorting Array", "[sort]") {
//   SortBench<int, MergeSort, Generator> bench{true, true};
//   std::size_t size = GENERATE(3, 10, 50, 100, 1000, 1000);
//   std::size_t measure_num = GENERATE(1, 5, 10);

//   bench(size, measure_num);

//   for (auto arr : bench.sorted_arrays()) {
//     REQUIRE(std::is_sorted(std::begin(arr), std::end(arr)) == true);
//   }
// }

// TEST_CASE("Quick Sorting Array", "[sort]") {
//   SortBench<int, QuickSort, Generator> bench{true, true};
//   std::size_t size = GENERATE(3, 10, 50, 100, 1000, 1000);
//   std::size_t measure_num = GENERATE(1, 5, 10);

//   bench(size, measure_num);

//   for (auto arr : bench.sorted_arrays()) {
//     REQUIRE(std::is_sorted(std::begin(arr), std::end(arr)) == true);
//   }
// }

// TEST_CASE("Heap Sorting Array", "[sort]") {
//   SortBench<int, HeapSort, Generator> bench{true, true};
//   std::size_t size = GENERATE(3, 10, 50, 100, 1000, 1000);
//   std::size_t measure_num = GENERATE(1, 5, 10);

//   bench(size, measure_num);

//   for (auto arr : bench.sorted_arrays()) {
//     REQUIRE(std::is_sorted(std::begin(arr), std::end(arr)) == true);
//   }
// }

// TEST_CASE("Counting Sorting Array", "[sort]") {
//   SortBench<int, CountingSort, Generator> bench{true, true};
//   std::size_t size = GENERATE(3, 10, 50, 100, 1000, 1000);
//   std::size_t measure_num = GENERATE(1, 5, 10);

//   bench(size, measure_num);

//   for (auto arr : bench.sorted_arrays()) {
//     REQUIRE(std::is_sorted(std::begin(arr), std::end(arr)) == true);
//   }
// }

// TEST_CASE("Radix Sorting Array", "[sort]") {
//   SortBench<int, RadixSort, Generator> bench{true, true};
//   std::size_t size = GENERATE(3, 10, 50, 100, 1000, 1000);
//   std::size_t measure_num = GENERATE(1, 5, 10);

//   bench(size, measure_num);

//   for (auto arr : bench.sorted_arrays()) {
//     REQUIRE(std::is_sorted(std::begin(arr), std::end(arr)) == true);
//   }
// }

// TEST_CASE("Bucket Sorting Array", "[sort]") {
//   SortBench<int, BucketSort, Generator> bench{true, true};
//   std::size_t size = GENERATE(3, 10, 50, 100, 1000, 1000);
//   std::size_t measure_num = GENERATE(1, 5, 10);

//   bench(size, measure_num);

//   for (auto arr : bench.sorted_arrays()) {
//     REQUIRE(std::is_sorted(std::begin(arr), std::end(arr)) == true);
//   }
// }
