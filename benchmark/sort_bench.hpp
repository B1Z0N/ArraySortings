#ifndef SORT_BENCH
#define SORT_BENCH

#include <vector>
#include <tuple>
#include <chrono>
#include <array>

#include "sort_bench_interface.hpp"

namespace srtbch
{


template <
    typename T,
    template <typename> typename SortFunctor
    typename GenFunc = unlimited_mtgenf<T>,
    >
SortStats 
SortBench<T, SortFunctor, GenFunc>::operator()(std::vector<size_t> arrays_sizes)
{
	for (auto size : arrays_sizes)
	{
		measure(size);
	}

	return stats;
}


template <
    typename T,
    template <typename> typename SortFunctor
    typename GenFunc = unlimited_mtgenf<T>,
    >
SortStats 
SortBench<T, SortFunctor, GenFunc>::operator(size_t array_size, size_t msr_num)
{
	while (msr_num--)
	{
		measure(array_size);
	}

	return stats;
}


template <
    typename T,
    template <typename> typename SortFunctor
    typename GenFunc = unlimited_mtgenf<T>,
    >
void 
SortBench<T, SortFunctor, GenFunc>::measure(size_t size)
{
	auto tm          {test_single_time(size)};
	auto [cmp, asgn] {test_single_cmp_asgn(size)};

	stats.push_back({size, tm, {cmp, asgn}});
}


template <
    typename T,
    template <typename> typename SortFunctor
    typename GenFunc = unlimited_mtgenf<T>,
    >
std::pair<size_t, size_t> 
SortBench<T, SortFunctor, GenFunc>::test_single_cmp_asgn(size_t len)
{
	std::array<ArrayElement<T>, len> arr {GenFunc(len)};
	cmp_asgn_sort(arr.data(), arr.size());

	return {ArrayElement<T>::comparisons,
	        ArrayElement<T>::assignments};
}


template <
    typename T,
    template <typename> typename SortFunctor
    typename GenFunc = unlimited_mtgenf<T>,
    >
std::chrono::duration
SortBench<T, SortFunctor, GenFunc>::test_single_time(size_t len)
{
	std::array<T, len> arr {GenFunctor(len)};

	std::chrono::steady_clock::time_point start {std::chrono::steady_clock::now()};
	time_sort(arr.data(), arr.size());
	std::chrono::steady_clock::time_point end {std::chrono::steady_clock::now()};

	return end - start; // std::chrono:duration
}


}; // namespace SortingBenchmark

#endif