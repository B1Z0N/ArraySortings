#include "catch.hpp"

#include "sorting_benchmark/sorting_benchmark.hpp"
#include "sorting_benchmark/sortings.hpp"
#include "sorting_benchmark/utility.hpp"

#include <utility>

using namespace srtbch;
using namespace sortings;

TEST_CASE("Sort Array", "[sort]") {

  SortBench<int, BubbleSort, Generator> bench{true, true};
  std::size_t size = GENERATE(3, 10, 50, 100, 1000, 1000);
  std::size_t measure_num = GENERATE(1, 5, 10);

  bench(size, measure_num);

  for (auto arr : bench.notsorted_arrays()) {
    CHECK_NOFAIL(std::is_sorted(std::begin(arr), std::end(arr)) == false);
  }
  for (auto arr : bench.sorted_arrays()) {
    REQUIRE(std::is_sorted(std::begin(arr), std::end(arr)) == true);
  }
}

TEST_CASE("Not Inited Throw", "[sort][throw]") {
  SortBench<int, BubbleSort, Generator> bench{true, true};
  REQUIRE_THROWS_AS(bench.notsorted_arrays(), std::logic_error);
  REQUIRE_THROWS_AS(bench.sorted_arrays(), std::logic_error);

  REQUIRE_THROWS_WITH(bench.sorted_arrays(),
                      "Uninitialized arrays, use operator() to fill it first");
  REQUIRE_THROWS_WITH(bench.sorted_arrays(),
                      "Uninitialized arrays, use operator() to fill it first");
}

TEST_CASE("Not Kept Arrays Throw", "[sort][throw]") {
  SortBench<int, BubbleSort, Generator> bench{false, true};
  bench(1, 1);

  REQUIRE_THROWS_AS(bench.notsorted_arrays(), std::logic_error);
  REQUIRE_THROWS_WITH(
      bench.notsorted_arrays(),
      "Call keep_before(), and than operator() to get this arrays");
  REQUIRE_NOTHROW(bench.sorted_arrays());

  bench.keep_before(true);
  bench.keep_after(false);
  REQUIRE_NOTHROW(bench.notsorted_arrays());
  REQUIRE_THROWS_AS(bench.sorted_arrays(), std::logic_error);
  REQUIRE_THROWS_WITH(
      bench.sorted_arrays(),
      "Call keep_after(), and than operator() to get this arrays");
}