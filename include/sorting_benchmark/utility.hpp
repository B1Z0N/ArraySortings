/** @file
 *  helper functionality, like genfuncs, checkers and others
 */

#ifndef SORT_BENCH_UTILITY_HPP
#define SORT_BENCH_UTILITY_HPP

#include <algorithm>
#include <random>
#include <array>
#include <functional>

namespace srtbch {

/**
 * Generates random numbers of type
 * std::mt19937::result_type (aka 'std::uint_fast32_t')
 */
class SeededMT19937 : public std::mt19937 {
public:
    SeededMT19937() {
        std::random_device rd;
        std::array<int, std::mt19937::state_size> seed_data;
        std::generate(seed_data.data(), seed_data.size(), std::ref(rd));
        std::seed_seq seq {std::begin(seed_data), std::end(seed_data)};
        std::mt19937 {seq};
    }
};

/** is sorted in ascending order by default
 *  less by default means "less first"
 */
template <typename T, typename Comparison = std::less<T>>
bool is_sorted(T* arr, std::size_t size, const Comparison& cmp = std::less<T>{}) {
  for (std::size_t i = 0; i < size - 1; ++i) {
    if (!cmp(arr[i], arr[i + 1])) return false;
  }
  return true;
}

}  // namespace srtbch

#endif  // SORT_BENCH_UTILITY_HPP
