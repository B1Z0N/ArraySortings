#include <iostream>
#include <functional>
#include <iterator>
#include <vector>

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
				{
					T temp = arr[j + 1];
					arr[j + 1] = arr[j];
					arr[j] = temp;
				}

	}
};



template <
    typename T
    >
std::ostream& __print_container(std::ostream& os, const T& val, const char* separator = ", ",
                                const char *inscope = "[", const char* outscope = "]", bool end = false)
// prints single value
// end - tells if this the last element in the sequence
{
	os << val;
	if (!end)
	{
		// if this is not the end
		// print separator too
		os << separator;
	}

	return os;
}

template <
    typename T,
    template <typename> typename C
    >
std::ostream& __print_container(std::ostream& os, const C<T>& con,
                                const char* separator = ", ", const char *inscope = "[", const char* outscope = "]", bool end = false)
// prints iterable of iterbles/values of ...
// end - tells if this the last element in the sequence
{
	if (con.empty())
	{
		return os << inscope << " " << outscope;
	}
	auto prev_end {std::rbegin(con)};

	os << inscope;

	for (auto it {std::begin(con)}; it != std::prev(prev_end.base()); it++)
	{
		__print_container(os, *it, separator, inscope, outscope);
	}

	__print_container(os, *prev_end, separator, inscope, outscope, true);

	os << outscope;

	if (!end)
	{
		// if this is not the end
		// print separator too
		os << separator << std::endl;
	}

	return os;
}

template <
    typename T,
    template <typename> typename C
    >
std::ostream& print_container(std::ostream& os, const C<T>& con,
                              const char* separator = ", ", const char *inscope = "[", const char* outscope = "]")
// wrapper for prettyness
{
	return __print_container(os, con, separator, inscope, outscope, false);
}


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

auto& standard_ssoutput(const srtbch::SortStats& ss) { return ssoutput(std::cout, ss); }


template <
    typename T,
    template <typename> typename Sort,
    typename GenF = srtbch::unlimited_mtgenf<T>
    >
void test_sort(size_t array_size, size_t measure_num)
{
	srtbch::SortBench<T, Sort, GenF> srt {true, true};

	auto res {srt(array_size, measure_num)};
	standard_ssoutput(res);
	std::cout << "Generated containers is: \n";
	print_container(std::cout, srt.notsorted_arrays());
	std::cout << "And sorted containers is: \n";
	print_container(std::cout, srt.sorted_arrays());
	std::cout << std::endl;
}

void test_sort_ints()
{
	test_sort<int, BubbleSort>(4, 3);
	// test_sort<int, BubbleSort>(25, 3);
	// test_sort<int, BubbleSort>(100, 3);
}

struct From	{
	constexpr static double value = 3.45;
};

struct To   {
	constexpr static double value = 5.942;
};


void test_sort_doubles()
{

	test_sort<double, BubbleSort,
	          srtbch::lim_unif_real_mtgenf_type<double, From, To>>(4, 3);

	// test_sort<int, BubbleSort>(25, 3);
	// test_sort<int, BubbleSort>(100, 3);
}

int main()
{
	test_sort_ints();
	test_sort_doubles();
}