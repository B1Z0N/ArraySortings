#include "sorting_benchmark/array_element.hpp"

#include "catch.hpp"

#include <random>
#include <utility>

using namespace srtbch;

struct ResetFixture {
  ResetFixture() {
    ArrayElement<int>::reset();
  }
  ArrayElement<int> ae1;
};


////////////////////////////////////////////////////////////
// Assignments tests
////////////////////////////////////////////////////////////


TEST_CASE_METHOD(ResetFixture, "Basic Single ArrayElement Assignment",
                 "[basic assignment][single array element]") {
  REQUIRE(ArrayElement<int>::get_asgn() == 0);
}

TEST_CASE_METHOD(ResetFixture, "Move Single ArrayElement Assignment",
                 "[move assignment][single array element]") {
  int a = 3;
  ae1 = std::move(a);

  REQUIRE(ArrayElement<int>::get_asgn() == 1);
}

TEST_CASE_METHOD(ResetFixture, "Increment Single ArrayElement Assignment",
                 "[fixed increment assignment][single array element]") {
  ae1 = 3;
  REQUIRE(ArrayElement<int>::get_asgn() == 1);
  ae1 = 3;
  REQUIRE(ArrayElement<int>::get_asgn() == 2);
}

TEST_CASE_METHOD(ResetFixture,
                 "Random Increment Single ArrayElement Assignment",
                 "[random increment assignment][single array element]") {
  // this test will be ran 5 times with random
  // maximal number of runs, from 1 to 1e5
  std::size_t num = GENERATE(take(5, random(1, static_cast<int>(1e5))));

  for (std::size_t i = 0; i < num; ++i) {
    ae1 = 3;
    REQUIRE(ArrayElement<int>::get_asgn() == i + 1);
  }
  REQUIRE(ArrayElement<int>::get_asgn() == num);
}

TEST_CASE_METHOD(
    ResetFixture, "Random Increment Multiple ArrayElement`s",
    "[random increment][multiple array elemens][move assignment]") {
  // this test will be ran 5 times with random
  // maximal number of runs, from 1 to 1e5
  std::size_t num = GENERATE(take(5, random(1, static_cast<int>(1e5))));

  for (std::size_t i = 0; i < num; ++i) {
    ArrayElement<int> ae2;
    ae2 = std::move(ae1);
    ae1 = 3;
    REQUIRE(ArrayElement<int>::get_asgn() == 2 * (i + 1));
  }
  REQUIRE(ArrayElement<int>::get_asgn() == 2 * num);
}


////////////////////////////////////////////////////////////
// Comparisons tests
////////////////////////////////////////////////////////////


static auto random_compare{[](const auto& fst, const auto& snd) {
  static std::mt19937 mt{std::random_device{}()};
  static std::uniform_int_distribution dis {1, 6};
  unsigned int choice = dis(mt);

  switch (choice) {
    case 1: {
      return fst > snd;
    }
    case 2: {
      return fst < snd;
    }
    case 3: {
      return fst >= snd;
    }
    case 4: {
      return fst <= snd;
    }
    case 5: {
      return fst == snd;
    }
    case 6: {
      return fst != snd;
    }
  }
}};

TEST_CASE_METHOD(ResetFixture, "Basic Single ArrayElement Comparison",
                 "[basic comparison][single array element]") {
  REQUIRE(ArrayElement<int>::get_asgn() == 0);
}

TEST_CASE_METHOD(ResetFixture, "Increment Single ArrayElement Comparison",
                 "[fixed increment comparison][single array element]") {
  ae1 > 3;
  REQUIRE(ArrayElement<int>::get_cmp() == 1);
  ae1 < 3;
  REQUIRE(ArrayElement<int>::get_cmp() == 2);
}

TEST_CASE_METHOD(ResetFixture,
                 "Random Increment Single ArrayElement Comparison",
                 "[random increment comparison][single array element]") {
  // this test will be ran 5 times with random
  // maximal number of runs, from 1 to 1e5
  std::size_t num = GENERATE(take(5, random(1, static_cast<int>(1e5))));

  for (std::size_t i = 0; i < num; ++i) {
    random_compare(ae1, 3);
    REQUIRE(ArrayElement<int>::get_cmp() == i + 1);
  }
  REQUIRE(ArrayElement<int>::get_cmp() == num);
}

TEST_CASE_METHOD(
    ResetFixture, "Random Increment Multiple ArrayElement`s Comparison",
    "[random increment][multiple array elemens]") {
  // this test will be ran 5 times with random
  // maximal number of runs, from 1 to 1e5
  std::size_t num = GENERATE(take(5, random(1, static_cast<int>(1e5))));

  for (std::size_t i = 0; i < num; ++i) {
    ArrayElement<int> ae2;
    random_compare(ae2, ae1);
    random_compare(ae1, 3);
    REQUIRE(ArrayElement<int>::get_cmp() == 2 * (i + 1));
  }
  REQUIRE(ArrayElement<int>::get_cmp() == 2 * num);
}


////////////////////////////////////////////////////////////
// Reset tests
////////////////////////////////////////////////////////////


TEST_CASE_METHOD(ResetFixture, "Assignments N Comparisons Reset", "[reset]") {
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