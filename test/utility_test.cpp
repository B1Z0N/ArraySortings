#include "catch.hpp"

#include "sorting_benchmark/utility.hpp"

#include <array>
#include <functional>

using namespace srtbch;

////////////////////////////////////////////////////////////
// is_sorted tests
////////////////////////////////////////////////////////////

TEST_CASE("Base cases", "[basic]") {
  std::array arr2{3};
  REQUIRE(is_sorted(arr2.data(), arr2.size()) == true);
  REQUIRE(is_sorted(arr2.data(), arr2.size(), std::greater<int>{}) == true);
}

TEST_CASE("Is Sorted In Ascending Order(Default)",
          "[is sorted][ascending][default]") {
  std::array arr{1, 3, 4, 6, 10, 23};
  REQUIRE(is_sorted(arr.data(), arr.size()) == true);
  arr[0] = 5;
  REQUIRE(is_sorted(arr.data(), arr.size()) == false);
  arr[0] = 1;
  REQUIRE(is_sorted(arr.data(), arr.size(), std::greater<int>{}) == false);
}
