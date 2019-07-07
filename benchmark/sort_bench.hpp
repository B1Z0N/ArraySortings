#ifndef SORT_BENCH
#define SORT_BENCH

#include <vector>
#include <tuple>
#include <chrono>
#include <array>
#include <functional>
#include <algorithm>
#include <stdexcept>


#include "sort_bench_interface.hpp"

namespace srtbch
{


template <
    typename T,
    template <typename> typename SortFunctor,
    typename GenFunc
    >
SortBench<T, SortFunctor, GenFunc>::SortBench(bool keep_arr) 
	:keep{keep_arr} {}


template <
    typename T,
    template <typename> typename SortFunctor,
    typename GenFunc
    >
void SortBench<T, SortFunctor, GenFunc>::keep_arrays()
{	
	keep = true;
}


template <
    typename T,
    template <typename> typename SortFunctor,
    typename GenFunc
    >
SortStats
SortBench<T, SortFunctor, GenFunc>::operator () (std::vector<size_t> arrays_sizes)
{
	sort_arrs.clear();
	for (auto size : arrays_sizes)
	{
		measure(size);
	}

	return stats;
}


template <
    typename T,
    template <typename> typename SortFunctor,
    typename GenFunc
    >
SortStats 
SortBench<T, SortFunctor, GenFunc>::operator () (size_t array_size, size_t msr_num)
{
	sort_arrs.clear();
	while (msr_num--)
	{
		measure(array_size);
	}

	return stats;
}


template <
    typename T,
    template <typename> typename SortFunctor,
    typename GenFunc
    >
std::vector<std::vector<T>>
SortBench<T, SortFunctor, GenFunc>::arrays()
{
	if (!is_inited) 
	{
		throw std::logic_error {
			"Uninitialized arrays, use operator() to fill it first"
		};
	}

	return sort_arrs;
}


template <
    typename T,
    template <typename> typename SortFunctor,
    typename GenFunc
    >
void 
SortBench<T, SortFunctor, GenFunc>::measure(size_t size)
{

	std::vector<T> tvec (size);
	std::generate(std::begin(tvec), std::end(tvec), std::ref(gen));
	auto tm {test_single_time(tvec)};

	{
	std::vector<ArrayElement<T>> cavec (size);
	std::transform(std::begin(tvec), std::end(tvec), 
				   std::begin(cavec), [](T elem) { 
				   					  return ArrayElement<T> {elem}; 
									 });
	auto [cmp, asgn] {test_single_cmp_asgn(cavec)};

	stats.push_back({size, tm, {cmp, asgn}});
	}

	if (keep)
	{
		sort_arrs.push_back(tvec);
		is_inited = true;
	}
}


template <
    typename T,
    template <typename> typename SortFunctor,
    typename GenFunc
    >
std::pair<size_t, size_t> 
SortBench<T, SortFunctor, GenFunc>::test_single_cmp_asgn(std::vector<ArrayElement<T>> vec)
{
	cmp_asgn_sort(vec.data(), vec.size());

	auto cmp  = ArrayElement<T>::get_cmp();
	auto asgn = ArrayElement<T>::get_asgn();

	ArrayElement<T>::reset();

	return {cmp, asgn};
}


template <
    typename T,
    template <typename> typename SortFunctor,
    typename GenFunc
    >
std::chrono::nanoseconds
SortBench<T, SortFunctor, GenFunc>::test_single_time(std::vector<T> vec)
{
	using namespace std::chrono;

	steady_clock::time_point start {steady_clock::now()};
	time_sort(vec.data(), vec.size());
	steady_clock::time_point end {steady_clock::now()};

	return nanoseconds(end - start); // std::chrono:nanoseconds
}


}; // namespace SortingBenchmark

#endif