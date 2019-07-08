

# ArraySortings
Repository with sorting methods and benchmarking facilities. 

[**Documentation**](https://b1z0n.github.io/SortingBenchmark/)

# Compilation
Use `g++ some.cpp -o some -std=c++17`.

Other compilers not guaranteed to work.

Include `benchmark/sort_bench.hpp`.
# Features
All of this, you could find in namespace `srtbch` (spell as SortBench)
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
## Element generation functions
Remember: **PRNG stands for "pseudorandom number generator"**
Remember: **Call `operator()` for generation**

There are several classes that provides this feature(generation):
### Classes
* `SeededPRNG<T, PRNG_T, size_t seed_len>` template class that generates seed values for PRNG. 
  * `T` - type for generating.
  * `PRNG_T` - type of generator, should have a constructor, that accepts two iterators of seed `[begin, end]` range.
  * `size_t seed_length` - length of seed iterable (nontype parameter).
`SeedePRNG` meets the requirements of [`RandomNumberEngine`](https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine)).
You could use it like this:
```c++
SeededPRNG<int, some_prng, 624> sprng {};
std::cout << "Generated value: " << sprng() << std::endl;
```
Doesn't matter what type of elements generates `PRNG_T` object, this PRNG turns them to `T`. Under the hood it is implemented with [`std::random_device`](https://en.cppreference.com/w/cpp/numeric/random/random_device)(for seed generating) and [`std::memcpy`](https://en.cppreference.com/w/cpp/string/byte/memcpy) for type conversion.

* `Genfunctor_base<InT, OutT, SeededPRNG_T>` - template class for generating values of type `OutT` with help of seeded generator of type `SeededPRNG_T` that generates values of type `InT`.
  * `InT` and `OutT` should be of numeric types (real or integral).
  * `SeededPRNG_T` must meet the requirements of [`RandomNumberEngine`](https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine). For classes that use heap, or mask their actual size(with pointers, for example) write your own manual generator, using this one.
Class meets the requirements of [`RandomNumberEngine`](https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine)).

* `LimGenFunctor_base<T, DistribT, GeneratorT>` - class for generating limited values of different types with help of: 
  * `T` - type being generated.
  * `DistribT` - distribution of generation, that meets the requirements of [`RandomNumberDistribution`](https://en.cppreference.com/w/cpp/named_req/RandomNumberDistribution). 
  * `GeneratorT` - generator, that meets the requirements of [`RandomNumberEngine`](https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine)).
It is the base class, that sets interface to other classes.
Constructor accepts values `const T& from` and `const T& to` that limit a range of generated values `[from, to]`.

* `LimGenFunctorVal<T, DistribT, GeneratorT, T from, T to>` - doing the same thing as `LimGenFunctor_base<T, DistribT, GeneratorT>`, except of nontype parameters `from` and `to`, both of type `T`:
  * `T from` - value that shows the beginning of the interval.
  * `T to` - value that shows the ending of the interval.
Constructor accepts no parameters. Call `operator()` for generation.
Values of some types couldn't be passed as nontype template parameter, double for example(precision problems) couldn't(according to C++17 standard(oh, i hope they will, i hope they will)). But this is an important strategical point - **sort variables of real types**. So the next class is a workaround to this problem.

* `LimGenFunctorType<T, DistribT, GeneratorT, From, To>` - doing the same thing as `LimGenFunctor_base<T, DistribT, GeneratorT>`, except of type parameters `From` and `To`:
  * `From` - type that has `constexpr static` member of type `T` named `value`, that shows the beginning of the interval. 
  * `To` - type that has `constexpr static` member of type `T` named `value`, that shows the ending of the interval. 
Constructor accepts no parameters.

This are classes, but the most useful for end user will be type aliases for this classes:
### Type aliases
First of all, all of this aliases are based on [mt19937 generator](https://en.wikipedia.org/wiki/Mersenne_Twister) and uniform [continuous](https://en.wikipedia.org/wiki/Uniform_distribution_(continuous)) or [discrete](https://en.wikipedia.org/wiki/Discrete_uniform_distribution) distribution.

* `SeededMT` - mt19937 ready for generation of type `uint32_t`, seeded with [`std::random_device`](https://en.cppreference.com/w/cpp/numeric/random/random_device)
* `GenFunctor<InT, OutT, PRNG_T, size_t seedlen>` - template class for creating own genfuncs
  * `InT` - type of seed values and generating of `PRNG_T` object.
  * `OutT` - type, values of which are about to be generated.
  * `PRNG_T` - type of generator, should have a constructor, that accepts two iterators of seed `[begin, end]` range.
  * `size_t seed_length` - length of seed iterable (nontype parameter).

* `unlimited_mtgenf<OutT>` - `mt19937` ready for generation of values, of type `OutT`
  * `OutT` - should be numeric (real or integral), for nonnumeric values - turn on your imagination(it is easier than you think).

* `lim_unif_int_mtgenf<IntT, IntT from, IntTo>` - spell as *limited, uniform distribution, integral, mersenne twister of seed length 19937 bits, generating function*
  * `IntT` - type, values of which are about to be generated, should be integral (e.g `short int`, `long long`).
  * `IntT from` - value that shows the beginning of the interval.
  * `IntT to` - value that shows the ending of the interval.
  
Class uses [`std::uniform_int_distribution`](https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution)

Usage:
```c++
lim_unif_int_mtgenf<int, 3, 6> gen {};

std::cout << gen() << std::endl; // 3, 4, 5 or 6 ?
```

* `lim_unif_real_mtgenf<RealT>` - limited generating function of real types
  * `IntT` - type, values of which are about to be generated, should be real (e.g `double`, `float`).
It's constructor accepts `RealT from` and `RealT to` - range of values to be generated. Looking in long perspective, you'll want to make this constructor take zero arguments, so that you could pass this class to `SortBench` as a template template parameter. But due to c++ limitations(see `LimGenFunctorVal` above) we can't do this. So my recomendation is that you have two options, first is:
```c++
class my_real_limited_e_pi : public lim_unif_real_mtgenf<double>
{
    using parent = lim_unif_real_mtgenf<double>;
public:
    my_real_limited_e_pi()
        : parent{2.718, 3.141} {}
};

// now you can pass this as a type to SortBench
```
And the other option is the next item:

* `lim_unif_real_mtgenf_type<RealT, From, To>` - limited generating function of real types
  * `From` - type that has `constexpr static` member of type `T` named `value`, that shows the beginning of the interval. 
  * `To` - type that has `constexpr static` member of type `T` named `value`, that shows the ending of the interval. 
Constructor accepts no parameters.

Like that:
```c++
struct FromE {
    constexpr static double value {2.718};
};

struct ToPi {
	constexpr static double value {3.141};
};

using my_lim_genf = lim_unif_real_mtgenf_type<double, FromE, ToPi>;
// now you can pass this as a type to SortBench
```
Both of the `lim_unif_real_mtgenf<RealT>` and `lim_unif_real_mtgenf_type<RealT, From, To>` use [`std::uniform_real_distribution`](https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution)

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
## Note about future
1. Planning to implement the very basic sortings.
2. Documentation site.
3. Implement concepts, or template type assertions.
