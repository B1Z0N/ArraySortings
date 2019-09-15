/** @file
 *  helper functionality, like genfuncs, checkers and others
 */

#ifndef SORT_BENCH_UTILITY_HPP
#define SORT_BENCH_UTILITY_HPP

#include <algorithm>
#include <array>
#include <functional>
#include <random>

namespace srtbch {

/**
 * Generates random numbers of type
 * std::mt19937::result_type (aka 'std::uint_fast32_t')
 */
class Generator : public std::mt19937 {
 public:
  Generator() {
    std::random_device rd;
    std::array<int, std::mt19937::state_size> seed_data;
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937{seq};
  }
};

}  // namespace srtbch

#endif  // SORT_BENCH_UTILITY_HPP
