#ifndef SORT_BENCH_INTERFACE
#define SORT_BENCH_INTERFACE

#include <vector>
#include <tuple>
#include <chrono>
#include <array>

#include "genfunc.hpp"

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
    typename GenFunc = GenFunctor<T>,
    >
// look forward to thread safety
// look forward to array + - and so on operations on ArrayElement, for sortings like RadixSort
class SortBench
{
    SortStats stats;

    Functor<T> time_sort;
    Functor<ArrayElement<T>> cmp_asgn_sort;
public:

    SortStats operator()(std::vector<size_t> arrays_sizes );
    SortStats operator()(size_t array_size, size_t msr_num);
private:

    void measure(size_t size);

    std::pair<size_t, size_t> test_single_cmp_asgn(size_t len);
    std::chrono:duration test_single_time(size_t len);
};


};
#endif