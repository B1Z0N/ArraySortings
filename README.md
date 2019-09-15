# SortingBenchmark

Repository with sorting methods and benchmarking facilities.

# Table of contents

- [SortingBenchmark](#sortingbenchmark)
- [Installation and testing](#installation-and-testing)
- [Using in your project](#using-in-your-project)
  * [Manually](#manually)
  * [CMake](#cmake)
- [Features](#features)
  * [ArrayElement](#arrayelement)
- [Utility](#utility)
  * [Benchmark](#benchmark)
- [Sortings](#sortings)
  * [Note about future](#note-about-future)


# Installation and testing

It is one header project, so just include appropriate file in folder named `bench.hpp` in `inculde/sorting_benchmark/` directory.

To run the tests(assuming you are in the root repository directory):
```bash
mkdir build  # or mybuild
cd build
cmake ..
make
./test_runner
```

# Using in your project

From previous step you can find static library generated in `build` directory. File named `libsrtbch.a` is a static library. So you should link this to your cpp-program and also add an include path which is in `include/sorting_benchmark/` folder in the project's root. 

## Manually

```c++
// use.cpp

#include "sorting_benchmark/bench.hpp"

using namespace srtbch;

int main() {
  SortBench<int, sortings::BubbleSort, Generator> bench;
  bench(3, 2); // generate array of length 3 and measure it 2 times
  for (const auto& vec : bench.sorted()) {
    for (const auto& x : vec) {
      std::cout << v << ", ";
    }
    std::cout << '\n';
  }
}
```

And compile-link-run it like this(gcc example):

```bash
> g++ use.cpp -I"${PROJECT_DIR}/inculde" ${PROJECT_DIR}/build/libsrtbch.a -o use
> ./use
> ... "some two arrays of length three" ...
```

Where `${PROJECT_DIR}` is a path to this project's directory.

## CMake

Add this lines to your `CMakeLists.txt`:

```cmake
include_directories(${NOTACONV_PROJECT_DIR}/include)
link_directories(${NOTACONV_PROJECT_DIR}/build)
...
...
...
target_link_libraries(executable srtbch)
```

Where `${NOTACONV_PROJECT_DIR}` is a path to this project's directory, and `executable` is your target specified in `add_executable(executable ${SOURCES})` function call.

# Features
Main things, you could find in namespace `srtbch` (spell as SortBench).

And another is sorting functions, which are in `srtbch::sorting` namespace.

## ArrayElement
Is a template class that helps with counting of comparisons and assignments operation. Designed to count this values automatically.
For example:
```c++
std::array<ArrayElement<int>, 10> arr{};
std::generate(std::begin(arr), std::end(arr), some_gen_func);

some_sort_func(arr.data(), arr.size());

std::cout << "Comparisons: " << ArrayElement<int>::get_cmp()  << std::endl;
std::cout << "Assignments: " << ArrayElement<int>::get_asgn() << std::endl;

ArrayElement<int>::reset();
// start counting from zero
```
You could manually diasble/enable some of the counting facilites:
```c++
ArrayElement<int>::off_on_cmp_count(true)   
// means turn on comparison counting

ArrayElement<int>::off_on_asgn_count(false) 
// means turn off assignment counting
```
Comparison and assignment operator support included. Looking forward to include some other operators for behaving much like T class itself. It will be usefull for testing some extraordinary sortings like radix sort. But that's ignored till this sortings being implemented(see "**Note about future**").

## Utility

This repo provides currently one function to generate pseudo-random numbers. It is `srtbch::Generator` and it generates only numbers of type `std::uint_fast32_t`(a.k.a `long unsigned int` on `gcc`). It was designed to be passed to `srtbch::SortBench`(see next item), but you could provide you implementation.

Basically it is just `std::mt19937` properly(624 bytes) seeded with `std::random_device` and `std::seed_seq`. Use it like this:

```c++
#include "bench.hpp"

#include <iostream>

using namespace srtbch;

int main() {
    Generator gen;
    long unsigned int random_number {gen()};	// generates number from 0 to 2^19937 - 1
    auto auto_random_number {gen()};			// use it also like this
}
```



## Benchmark
`SortBench` is a class that helps in testing sorting functions, full declaration:
`template <typename T, template <typename> typename SortFunctor, typename GenFunc = unlimited_mtgenf<T>>`, where:
* `T` - type to be generated and sorted. 
* `SortFunctor` - template that sorts arrays of type `T`, it should have `operator(T * array, size_t size)`.
* `GenFunc` - class, that generates values of type `T`(for `unlimited_mtgenf` and other interesting genfunctors look above).

Simple usecase:
```c++
template <typename T>
struct BubbleSort
// functors only supported
// due to c++ limitations
{
	  void operator()(T *arr, uint len) noexcept
      // should have this method
	  {
		  for (uint i = 0; i < len - 1; i++)
			  for (uint j = 0; j < len - i - 1; j++)
			  	if (arr[j] > arr[j + 1])
				  	std::swap(arr[j], arr[j + 1])
	  }
};

SortBench<int, BubbleSort> srt;

auto res {srt(4 /*array_size*/, 3 /*measure_num*/)};
// 3 different arrays of size 4 being generated and tested

output_sort_stats(res);
// res of type SortStats

std::cout << "Generated containers is: \n";
print_container(std::cout, srt.notsorted_arrays());
// container of start(notsorted) arrays

std::cout << "And sorted containers is: \n";
print_container(std::cout, srt.sorted_arrays());
// container of end(sorted) arrays

std::cout << std::endl;
}
```
For print_container function see [this](https://gist.github.com/B1Z0N/e122def9e42e2ee198519bb70642feda).

Now that we've seen basic example, let's light the details. 
There are such helper classes/aliases:

* `CmpAsgn` - assembles comparisons and assignments in one structure.
   * `size_t cmp`  - comparisons.
   * `size_t asgn` - assignments.
* `SortStats` - declared as follows:
```c++
using SortStats = std::vector<std::tuple<
                  size_t,                     // array length
                  std::chrono::nanoseconds,   // sorting duration
                  CmpAsgn                     // comparisons and assignments
                  >>;
```
And here is the code, potentially useful for you:
```c++
std::ostream& ssoutput(std::ostream& os, const srtbch::SortStats& ss)
{
		// code for the output of the data
		// returned from SortBench::operator() call
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
```

Main method of `SortBench` is `operator(...)`, it has two versions:
* `SortStats operator()(std::vector<size_t> array_sizes)`, where `array_sizes` is array of sizes to be generated and tested
* `SortStats operator()(size_t array_size, size_t measure_num)`, where:
   * `array_size` - size of every array being generated
   * `measure_num` - number of generations of arrays of size `array_size`

Also if you want to keep start or end arrays you should state this explicitly with one of this methods:
* `void keep_before(bool should = true)` to keep start arrays
* `void keep_after (bool should = true)` to keep end arrays
*  or via constructor: `SortBench(bool keep_before = false, bool keep_after = false)`

To access it, you should use appropriate methods:
* `std::vector<std::vector<T>> notsorted_arrays()`
* `std::vector<std::vector<T>> sorted_arrays()`
## Sortings

Repo provides six common sorting algorithms for use with `SortBench`. Use this way:

```c++
#include "bench.hpp"

#include <vector>
#include <algorithm>

using namespace srtbch;

template <template <typename> typename SortFunctor>
void sort_copy(int* arr, std::size_t size) {
	SortFunctor<int> sf;
	int* local_arr = new int[size];
	std::copy(arr, arr + size, local_arr);
  
	sf(local_arr, size);  // sorts
}

int main() {
	// c++17 only
	std::vector arr {3, 9, 11, -5, 5, 7};
  
	sort_copy<sortings::SelectionSort>(arr.data(), arr.size());
	sort_copy<sortings::BubbleSort>(arr.data(), arr.size());
	sort_copy<sortings::InsertionSort>(arr.data(), arr.size());
	sort_copy<sortings::MergeSort>(arr.data(), arr.size());
	sort_copy<sortings::QuickSort>(arr.data(), arr.size());
	sort_copy<sortings::HeapSort>(arr.data(), arr.size());
}
```



## Note about future

1. Implement concepts, or template type assertions.
