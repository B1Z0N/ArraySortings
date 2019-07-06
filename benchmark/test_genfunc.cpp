#include <vector>
#include <random>
#include <iostream>
#include <algorithm>


#include "genfunc_interface.hpp"

// using namespace std;
// using namespace srtbch;


template <typename Functor, typename OutT>
void unlimited_generator_test()
{
	Functor sprng {};
	std::vector<OutT> vec(10);
	std::generate(std::begin(vec), std::end(vec), std::ref(sprng));

	std::ostream_iterator<OutT> oi {std::cout, " "};
	std::copy(std::begin(vec), std::end(vec), oi);
	std::cout << std::endl;
}

void test_SeededPRNG()
{
	unlimited_generator_test<srtbch::SeededMT, int>();
}

void test_GenFunctor()
{
	unlimited_generator_test<srtbch::unlimited_mtgenf<int>, int>();
	unlimited_generator_test<srtbch::unlimited_mtgenf<char>, char>();
	unlimited_generator_test<srtbch::unlimited_mtgenf<double>, double>();
}

template <
	typename OutT, 
	template <typename> typename Functor
	>
void limited_generator_test(OutT min, OutT max)
{
	Functor<OutT> sprng {min, max};
	std::vector<OutT> vec(10);
	std::generate(std::begin(vec), std::end(vec), std::ref(sprng));

	std::ostream_iterator<OutT> oi {std::cout, " "};
	std::copy(std::begin(vec), std::end(vec), oi);
	std::cout << std::endl;
}

void test_limited_int()
{
	limited_generator_test<int, srtbch::lim_unif_int_mtgenf>(5, 9);
	limited_generator_test<int, srtbch::lim_unif_int_mtgenf>(-10, 5);
}

void test_limited_real()
{
	limited_generator_test<double, srtbch::lim_unif_real_mtgenf>(5, 9);
	limited_generator_test<double, srtbch::lim_unif_real_mtgenf>(-3.45, 5.23);
}

class VisibleCharGen
// example of definition of custom class with closed  range values
// using basic classes from the library
{
	static constexpr int code_of_minimal_visible_char {33};
	static constexpr int code_of_maximal_visible_char {94};

	srtbch::lim_unif_int_mtgenf<uint8_t> gen {
		code_of_minimal_visible_char,
		code_of_maximal_visible_char
	};
public:
	using result_type = char;

	auto min() const { return static_cast<char>(code_of_minimal_visible_char); }
	auto max() const { return static_cast<char>(code_of_maximal_visible_char); }

	char operator()()
	{
		return static_cast<char>(gen());
	}
};

int main()
{
	test_SeededPRNG();
	test_GenFunctor();
	test_limited_int();
	test_limited_real();

	srtbch::unlimited_mtgenf<char> sprng {};
	std::vector<char> vec(10);
	std::generate(std::begin(vec), std::end(vec), std::ref(sprng));

	std::ostream_iterator<int> oi {std::cout, " "};
	std::transform(std::begin(vec), std::end(vec), oi, [](char s) {return (int) s;});
	std::cout << std::endl;
	std::ostream_iterator<char> io {std::cout, " "};
	std::transform(std::begin(vec), std::end(vec), io, [](char s) {return s;});
	std::cout << std::endl;
	// std::cout << reinterpret_cast<char>(3);

	unlimited_generator_test<VisibleCharGen, char>();
	return 0;
}

