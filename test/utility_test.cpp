#include "catch.hpp"

#include "sorting_benchmark/utility.hpp"

#include <functional>
#include <array>

using namespace srtbch;

TEST_CASE("Is Sorted In Ascending Order(Default)", "[is sorted][ascending][default]") {
  std::array arr {1, 3, 4, 6 ,10, 23};
  REQUIRE(is_sorted(arr.data(), arr.size()) == true);
}