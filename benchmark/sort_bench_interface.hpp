#include <tuple>  // std::tuple
#include <vector> // std::vector

namespace SortingBenchmark
{

template <typename Array, typename ArrayElement>
using sortf = void (*)(Array<ArrayElement> &); // pointer to sorting function

template <typename ArrayElement> // pointer to function, that generates elements in range (min, max)
using genfunc = ArrayElement (*)(ArrayElement min, ArrayElement max);

template <typename ArrayElement>                     // pointer to function, that does the same
using const_genfunc = genfunc<const ArrayElement &>; // but passing const refs, because of cheapness of this operation

using uint = unsigned int;

template <typename ArrayElement>
class GenFunc
// class for supporting 2 types of generating functions:
// with const refs arguments and with ordinary arguments
// allows calling function not even thinking about what it's type
{
    bool type; // 0 - first(gf), 1 - second(cgf)
    union {
        genfunc gf;
        const_genfunc cgf;
    };

public:
    ArrayElement min;
    ArrayElement max;

    GenFunc() = delete;
    GenFunc(genfunc<ArrayElement>);       // constructor for 1st type
    GenFunc(const_genfunc<ArrayElement>); // constructor for 2nd type

    ArrayElement operator()(); // for calling just like ordinary generating function
}

template <typename Array, typename ArrayElement>
class SortFunc
{
    typedef sortf<Array, ArrayElement> cur_sortf;
    // precondition functions that return values and take only array

    // preconditions as the results of precondition functions
    // sorting function that return nothing and take only array

public:
    SortFunc() = delete;
    SortFunc()
}

template <typename T>
class ArrayElement
// class that allows comparison and assignment counting
{
    T elem;

public:
    // implements operators
}

template <typename Array, typename ArrayElement>
class SortBench
// implements time testing
// implements comparison and assignment counting
{
    uint amount;
    GenFunc<ArrayElement> generating;

public:
};
} // namespace SortingBenchmark