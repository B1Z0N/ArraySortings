/** @file
 *  interface to benchmarking facilites
 */

#ifndef SORT_BENCH_HPP
#define SORT_BENCH_HPP

#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "array_element.hpp"

namespace srtbch {

struct CmpAsgn {
  size_t cmp;
  size_t asgn;
};

/** convenience alias for result of work of SortBench::operator() */
using SortStats =
    std::vector<std::tuple<size_t,                    // array length
                           std::chrono::nanoseconds,  // sorting duration
                           CmpAsgn  // comparisons and assignments
                           >>;

/** main class that measures sorting time and amount of
 *  comparisons and assignments of this sorting, depeneds on this template
 * arguments:\n T - type of elements in array\n SortFunctor - class template,
 * that has operator(T *arr, size_t size) method\n GenFunc - template parameter,
 * that has operator() generating values of type T
 */
template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
class SortBench {
  SortStats stats;

  SortFunctor<T> time_sort;
  SortFunctor<ArrayElement<T>> cmp_asgn_sort;
  GenFunc gen{};

  std::vector<std::vector<T>> notsorted_arrs;
  bool keep_bef;

  std::vector<std::vector<T>> sorted_arrs;
  bool keep_aft;

  bool is_inited{false};

 public:
  SortBench(bool keep_before = false, bool keep_after = false);

  void keep_before(bool should = true);
  void keep_after(bool should = true);

  SortStats operator()(std::vector<size_t> array_sizes);
  SortStats operator()(size_t array_size, size_t measure_num);

  std::vector<std::vector<T>> notsorted_arrays();
  std::vector<std::vector<T>> sorted_arrays();

 private:
  void clear_data();

  void measure(size_t sz);

  std::pair<size_t, size_t> test_single_cmp_asgn(std::vector<ArrayElement<T>>&);
  std::chrono::nanoseconds test_single_time(std::vector<T>&);
};

template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
SortBench<T, SortFunctor, GenFunc>::SortBench(
    bool keep_before, bool keep_after)  // false - default
    : keep_bef{keep_before}, keep_aft{keep_after} {}

template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
void SortBench<T, SortFunctor, GenFunc>::keep_before(
    bool should)  // false - default
{
  keep_bef = should;
}

template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
void SortBench<T, SortFunctor, GenFunc>::keep_after(
    bool should)  // false - default
{
  keep_aft = should;
}

template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
SortStats SortBench<T, SortFunctor, GenFunc>::operator()(
    std::vector<size_t> arrays_sizes) {
  clear_data();
  for (auto size : arrays_sizes) {
    measure(size);
  }

  return stats;
}

template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
SortStats SortBench<T, SortFunctor, GenFunc>::operator()(size_t array_size,
                                                         size_t msr_num) {
  clear_data();
  while (msr_num--) {
    measure(array_size);
  }

  return stats;
}

template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
std::vector<std::vector<T>>
SortBench<T, SortFunctor, GenFunc>::notsorted_arrays() {
  if (!is_inited) {
    throw std::logic_error{
        "Uninitialized arrays, use operator() to fill it first"};
  }

  if (!keep_bef) {
    throw std::logic_error{
        "Call keep_before(), and than operator() to get this arrays"};
  }

  return notsorted_arrs;
}

template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
std::vector<std::vector<T>>
SortBench<T, SortFunctor, GenFunc>::sorted_arrays() {
  if (!is_inited) {
    throw std::logic_error{
        "Uninitialized arrays, use operator() to fill it first"};
  }

  if (!keep_aft) {
    throw std::logic_error{
        "Call keep_after(), and than operator() to get this arrays"};
  }

  return sorted_arrs;
}

/** clear previous data */
template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
void SortBench<T, SortFunctor, GenFunc>::clear_data() {
  stats.clear();
  sorted_arrs.clear();
  notsorted_arrs.clear();
}

/** do time and (comparsons and assignments) testings*/
template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
void SortBench<T, SortFunctor, GenFunc>::measure(size_t size) {
  std::vector<T> tvec(size);  // time vector
  std::generate(std::begin(tvec), std::end(tvec), std::ref(gen));

  if (keep_bef) {
    notsorted_arrs.push_back(tvec);  // backup notsorted array
    is_inited = true;                // it is inited now
  }

  {  // scope start

    std::vector<ArrayElement<T>> cavec(size);  // cmp asgn vector
    std::transform(std::begin(tvec), std::end(tvec), std::begin(cavec),
                   [](T elem) { return ArrayElement<T>{elem}; });

    auto tm{test_single_time(tvec)};
    // time test with "bare"(of type T) elements

    auto [cmp, asgn]{test_single_cmp_asgn(cavec)};
    // comparisons & assignments test with the same array
    // test with array of ArrayElement<T>

    stats.push_back({size, tm, {cmp, asgn}});
    // add measurements

  }  // scope end for minimize usage of memory(lifetime of cavec ends here)

  if (keep_aft) {
    sorted_arrs.push_back(tvec);  // add sorted array to sort_arrays
    is_inited = true;             // it is inited now
  }
}

template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
std::pair<size_t, size_t>
SortBench<T, SortFunctor, GenFunc>::test_single_cmp_asgn(
    std::vector<ArrayElement<T>>& vec) {
  cmp_asgn_sort(vec.data(), vec.size());

  auto cmp = ArrayElement<T>::get_cmp();
  auto asgn = ArrayElement<T>::get_asgn();

  ArrayElement<T>::reset();

  return {cmp, asgn};
}

template <typename T, template <typename> typename SortFunctor,
          typename GenFunc>
std::chrono::nanoseconds SortBench<T, SortFunctor, GenFunc>::test_single_time(
    std::vector<T>& vec) {
  using namespace std::chrono;

  steady_clock::time_point start{steady_clock::now()};
  time_sort(vec.data(), vec.size());
  steady_clock::time_point end{steady_clock::now()};

  return nanoseconds(end - start);  // std::chrono:nanoseconds
}

};  // namespace srtbch

#endif  // SORT_BENCH_HPP