#ifndef AOS_LAB2_SORT_BENCH_INTERFACE
#define AOS_LAB2_SORT_BENCH_INTERFACE

#include <vector>
#include <tuple>
#include <chrono>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cassert>
#include <array>
#include <iterator>

namespace srtbch
{


struct CmpAsgn
{
    size_t cmp;
    size_t asgn;
};


using SortStats = std::vector<std::tuple<
                  size_t,                     // array length
                  std::chrono::duration,      // sorting duration
                  CmpAsgn                     // comparisons and assignments
                  >>


template <
    typename T,
    template <typename> typename SortFunctor    
    typename LimitFunc
    typename GenFunc = GenFunctor<T>,
    >
// set limits on types and maybe even concepts
// look forward to thread safety
// look forward to array + - and so on operations on ArrayElement, for sortings like RadixSort
// write own generating function
class SortBench
{
    SortStats stats;

    Functor<T> time_sort;
    Functor<ArrayElement<T>> cmp_asgn_sort;
public:

    operator()(std::vector<size_t> arrays_sizes)
    {
        for (auto size : arrays_sizes)
        {
            measure(size);
        }
    }

    operator()(size_t array_size, size_t msr_num)
    {
        while (msr_num--)
        {
            measure(array_size);
        }
    }

private:
    void measure(size_t size)
    {
        auto tm          {test_single_time(size)};
        auto [cmp, asgn] {test_single_cmp_asgn(size)};

        stats.push_back({size, tm, {cmp, asgn}});
    }

    std::pair<size_t, size_t> test_single_cmp_asgn(size_t len)
    {
        std::array<ArrayElement<T>, len> arr {LimitFunc(GenFunc(len))};
        cmp_asgn_sort(arr.data(), arr.size());

        return {ArrayElement<T>::comparisons,
                ArrayElement<T>::assignments};
    }

    auto test_single_time(size_t len)
    {
        std::array<T, len> arr {GenFunctor(len)};

        std::chrono::steady_clock::time_point start {std::chrono::steady_clock::now()};
        time_sort(arr.data(), arr.size());
        std::chrono::steady_clock::time_point end {std::chrono::steady_clock::now()};

        return end - start; // std::chrono:duration
    }
};
};
#endif