#include <algorithm>
#include <vector>
#include <iostream>

#include "array_element.hpp"

template <typename T>
using arrel = srtbch::ArrayElement<T>;

int main()
{
	// auto test_implicit_conversion {[](arrel<int>){}};

	// test_implicit_conversion(3);

	// std::cout << (arrel<int> {3} == 3);

	//----------------------------------------------

	std::vector<arrel<int>> vec {1, 2, 3, 4};
	std::find(std::begin(vec), std::end(vec), 4);

	std::cout << "Comparisons: " << arrel<int>::get_cmp() << std::endl;
	std::cout << "Assignments: " << arrel<int>::get_asgn() << std::endl;

	return 0;
}