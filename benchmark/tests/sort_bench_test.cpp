#include <iostream>
#include <functional>

#include "../sort_bench_interface.hpp"
// #include "../../simple_sorting_methods/bubble_sort.hpp"

template <typename T>
struct BubbleSort
{
	void operator()(T *arr, uint len) noexcept
	{
    	for (uint i = 0; i < len - 1; i++)
        	for (uint j = 0; j < len - i - 1; j++)
            	if (arr[j] > arr[j + 1])
                	std::swap(arr[j], arr[j + 1]);
    }
}

int main()
{
	srtbch::SortBench<int, BubbleSort> {};
}