#include "sorting_benchmark/array_element.hpp"

#include "catch.hpp"

#include <random>
#include <utility>

using namespace srtbch;

////////////////////////////////////////////////////////////
// Assignments tests
////////////////////////////////////////////////////////////

TEST_CASE(
    "Assignment"
    "[basic assignment] [single array element]") {
  ArrayElement<int>::reset();

  ArrayElement<int> ae1;

  REQUIRE(ArrayElement<int>::get_asgn() == 0);
}

TEST_CASE(
    "Assignment"
    "[move assignment] [single array element]") {
  ArrayElement<int>::reset();

  ArrayElement<int> ae1;
  int a = 3;
  ae1 = std::move(a);

  REQUIRE(ArrayElement<int>::get_asgn() == 1);
}

TEST_CASE(
    "Assignment"
    "[fixed increment assignment] [single array element]") {
  ArrayElement<int>::reset();

  ArrayElement<int> ae1;

  ae1 = 3;
  REQUIRE(ArrayElement<int>::get_asgn() == 1);
  ae1 = 3;
  REQUIRE(ArrayElement<int>::get_asgn() == 2);
}

TEST_CASE(
    "Assignment"
    "[random increment assignment][single array element]") {
  ArrayElement<int>::reset();

  // this test will be run 5 times with this
  // maximal number of runs (randomly generated)
  int max_num = GENERATE(1e3, 1e4, 1e5);

  ArrayElement<int> ae1;
  std::mt19937 mt{std::random_device{}()};
  std::uniform_int_distribution<int> dis{1, max_num};
  std::size_t num = dis(mt);

  for (std::size_t i = 0; i < num; ++i) {
    ae1 = mt();
    REQUIRE(ArrayElement<int>::get_asgn() == i + 1);
  }
  REQUIRE(ArrayElement<int>::get_asgn() == num);
}

////////////////////////////////////////////////////////////
// Comparisons tests
////////////////////////////////////////////////////////////

TEST_CASE(
    "Comparison"
    "[basic comparison] [single array element]") {
  ArrayElement<int>::reset();

  ArrayElement<int> ae1;

  REQUIRE(ArrayElement<int>::get_cmp() == 0);
}

TEST_CASE(
    "Comparison"
    "[all comparison operators] [single array element]") {
  ArrayElement<int>::reset();

  ArrayElement<int> ae1;
  REQUIRE(ArrayElement<int>::get_cmp() == 0);
  (void) (ae1 > 3);
  REQUIRE(ArrayElement<int>::get_cmp() == 1);
  (void) (ae1 < 3);
  REQUIRE(ArrayElement<int>::get_cmp() == 2);
  (void) (ae1 >= 3);
  REQUIRE(ArrayElement<int>::get_cmp() == 3);
  (void) (ae1 <= 3);
  REQUIRE(ArrayElement<int>::get_cmp() == 4);
  (void) (ae1 == 3);
  REQUIRE(ArrayElement<int>::get_cmp() == 5);
  (void) (ae1 != 3);
  REQUIRE(ArrayElement<int>::get_cmp() == 6);
}

TEST_CASE(
    "Comparison"
    "[fixed increment comparison] [single array element]") {
  ArrayElement<int>::reset();

  ArrayElement<int> ae1;

  (void)(ae1 > 3);
  REQUIRE(ArrayElement<int>::get_cmp() == 1);
  (void)(ae1 < 3);
  REQUIRE(ArrayElement<int>::get_cmp() == 2);
}

// TEST_CASE(
//     "Comparison"
//     "[random increment comparison][single array element]") {
//   ArrayElement<int>::reset();

//   // this test will be run 5 times with this
//   // maximal number of runs (randomly generated)
//   int max_num = GENERATE(1e3, 1e4, 1e5);

//   ArrayElement<int> ae1;
//   std::mt19937 mt{std::random_device{}()};
//   std::uniform_int_distribution<int> dis{1, max_num};
//   std::size_t num = dis(mt);

//   for (std::size_t i = 0; i < num; ++i) {
//     ae1 = mt();
//     REQUIRE(ArrayElement<int>::get_cmp() == i + 1);
//   }
//   REQUIRE(ArrayElement<int>::get_cmp() == num);
// }

// TEST_CASE(
//     "Comparison"
//     "[basic comparison] [single array element]") {
//   ArrayElement<int>::reset();

//   ArrayElement<int> ae1;

//   REQUIRE(ArrayElement<int>::get_cmp() == 0);
// }

// TEST_CASE(
//     "Comparison"
//     "[fixed increment comparison] [single array element]") {
//   ArrayElement<int>::reset();

//   ArrayElement<int> ae1;

//   ae1 = 3;
//   REQUIRE(ArrayElement<int>::get_cmp() == 1);
//   ae1 = 3;
//   REQUIRE(ArrayElement<int>::get_cmp() == 2);
// }

// TEST_CASE(
//     "Comparison"
//     "[random increment comparison][single array element]") {
//   ArrayElement<int>::reset();

//   // this test will be run 5 times with this
//   // maximal number of runs (randomly generated)
//   int max_num = GENERATE(1e3, 1e4, 1e5);

//   ArrayElement<int> ae1;
//   std::mt19937 mt{std::random_device{}()};
//   std::uniform_int_distribution<int> dis{1, max_num};
//   std::size_t num = dis(mt);

//   for (std::size_t i = 0; i < num; ++i) {
//     ae1 = mt();
//     REQUIRE(ArrayElement<int>::get_cmp() == i + 1);
//   }
//   REQUIRE(ArrayElement<int>::get_cmp() == num);
// }

////////////////////////////////////////////////////////////
// Reset tests
////////////////////////////////////////////////////////////

TEST_CASE("Assignments N Comparisons Reset", "[reset]") {
  ArrayElement<int>::reset();

  ArrayElement<int> ae1;
  ae1 = 3;
  REQUIRE(ArrayElement<int>::get_asgn() > 0);
  REQUIRE(ArrayElement<int>::get_cmp() == 0);
  ArrayElement<int>::reset();
  REQUIRE(ArrayElement<int>::get_asgn() == 0);
  REQUIRE(ArrayElement<int>::get_cmp() == 0);

  (void)(ae1 > 3);
  REQUIRE(ArrayElement<int>::get_asgn() == 0);
  REQUIRE(ArrayElement<int>::get_cmp() > 0);
  ArrayElement<int>::reset();
  REQUIRE(ArrayElement<int>::get_asgn() == 0);
  REQUIRE(ArrayElement<int>::get_cmp() == 0);
}