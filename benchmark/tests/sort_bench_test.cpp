#include <iostream>
#include <functional>
#include <iterator>

#include "../sort_bench.hpp"
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
};

std::ostream& ssoutput(std::ostream& os, const srtbch::SortStats& ss)
{
	using namespace std::chrono;

	for (auto &[sz, tm, ca] : ss)
	{
		os << "size: " << sz << '.';
		os << " time: " << (tm).count() << "ns.";
		os << " comparisons: " << ca.cmp << '.';
		os << " assignments: " << ca.asgn << '.' << std::endl;
	}

	return os;
}

std::ostream& standard_ssoutput(const srtbch::SortStats& ss) { return ssoutput(std::cout, ss); }

template <typename T, template <typename> typename C>
std::ostream& print_container(std::ostream& os, C<T> con)
{
	os << "[";
	std::ostream_iterator<T> oi {os, ", "};
	std::copy(std::begin(con), std::end(con), oi);
	os << "]";

	return os;
}

template <
	typename T, 
	template <typename> typename Sort, 
	typename GenF = srtbch::unlimited_mtgenf<T>
	>
void test_sort(size_t array_size, size_t measure_num)
{
	srtbch::SortBench<T, Sort, GenF> srt {};

	auto res {srt(array_size, measure_num)};
	standard_ssoutput(res);
	std::cout << "And sorted container is: \n";
	print_container(std::cout, srt.get_array());
	std::cout << std::endl;
}

void test_sort_ints()
{
	test_sort<int, BubbleSort>(4, 3);
	test_sort<int, BubbleSort>(25, 3);
	test_sort<int, BubbleSort>(100, 3);
}

int main()
{
	test_sort_ints();
}