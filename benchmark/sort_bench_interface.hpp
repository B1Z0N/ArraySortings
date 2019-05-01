#ifndef AOS_LAB2_SORT_BENCH_INTERFACE

#define AOS_LAB2_SORT_BENCH_INTERFACE
#include <tuple>  // std::tuple
#include <vector> // std::vector

namespace SortingBenchmark
{

template <template <typename> typename Array, typename ArrayElement>
using sortf = void (*)(Array<ArrayElement> &); // pointer to sorting function

using uint = unsigned int;

template <typename ArrayElement>
class GenFunc
// class for supporting 2 types of generating functions:
// with const refs arguments and with ordinary arguments
// allows calling function not even thinking about what it's type
{
    // pointer to function, that generates elements in range (min, max)
    using genf = ArrayElement (*)(ArrayElement min, ArrayElement max);

    // pointer to function, that does the same
    // but passing const refs, because of cheapness of this operation
    using const_genf = ArrayElement (*)(const ArrayElement &, const ArrayElement &);

    bool type; // 0 - first(gf), 1 - second(cgf)
    union {
        genf gf;
        const_genf cgf;
    };

public:
    ArrayElement min;
    ArrayElement max;

    GenFunc() = delete;
    GenFunc(genf genf, ArrayElement _min, ArrayElement _max);                     // constructor for 1st type
    GenFunc(const_genf genf, const ArrayElement &_min, const ArrayElement &_max); // constructor for 2nd type

    ArrayElement operator()(); // for calling just like ordinary generating function
};

// template <typename Array, typename ArrayElement, typename ReturnType>
// using precondf = ReturnType (*)(const Array<ArratElement>&);

// template <typename Array, typename ArrayElement>
// using precondf = ReturnType (*)(const Array<ArratElement>&);

template <template <typename> typename Array, typename ArrayElement /*, typename ...PreconditionReturnTypes */>
class SortFunc
{
    // template <typename ReturnType>
    // using precondf = ReturnType (*)(const Array<ArrayElement>&);

    using sortf = void (*)(Array<ArrayElement> &);

    // precondition functions that return values and take only array
    // std::tuple<precondf<PreconditionReturnTypes>...> preconditions;
    // std::tuple<PreconditionReturnTypes...> returns;
    // sorting function that returns nothing and take only array
    sortf sorting;

public:
    Array<ArrayElement> *arr;

    class ArrayNotGiven
    {
    };

    SortFunc() = delete;
    SortFunc(sortf);

    // void precond();
    void sort();
};

template <typename T>
class ArrayElement
// class that allows comparison and assignment counting
{
    T elem;
    static uint comparisons; // 0 by default
    static uint assignments; // 0 by default
public:
    static bool cmp_on;  // true by default
    static bool asgn_on; // true by default

    ArrayElement() = delete;
    ArrayElement(const T &);
    ArrayElement(T &&);

    ArrayElement &operator=(const ArrayElement &);
    ArrayElement &operator=(const T &);
    ArrayElement &operator=(ArrayElement &&);
    ArrayElement &operator=(T &&);

    void reset();

    template <typename U>
    friend bool operator==(const ArrayElement<U> &, const ArrayElement<U> &);
    template <typename U>
    friend bool operator!=(const ArrayElement<U> &, const ArrayElement<U> &);
    template <typename U>
    friend bool operator<(const ArrayElement<U> &, const ArrayElement<U> &);
    template <typename U>
    friend bool operator>(const ArrayElement<U> &, const ArrayElement<U> &);
    template <typename U>
    friend bool operator<=(const ArrayElement<U> &, const ArrayElement<U> &);
    template <typename U>
    friend bool operator>=(const ArrayElement<U> &, const ArrayElement<U> &);
};

template <typename ArrayElement>
struct SortStats
{
    time_t time;
    uint len;
    
    ArrayElement min;
    ArrayElement max;

    uint comparisons;
    uint assignments;
}

template <template <typename> typename Array, typename ArrayElement>
class SortBench
// implements time testing
// implements comparison and assignment counting
{
    using SortFunc<Array, ArrayElement>::sortf;
    using GenFunc<ArrayElement>::genf;
    using GenFunc<ArrayElement>::const_genf;

    SortStats<ArrayElement> stats;
    GenFunc<ArrayElement> generating;
    SortFunc<Array, ArrayElement> sorting;
    Array<ArrayElement> array;

protected:
    void find_comparisons_and_assignments();
    void time_it();
public:
    SortBench() = delete;
    SortBench(sortf sort, genf generate, ArrayElement min, ArrayElement max);
    SortBench(sortf sort, const_genf generate, const ArrayElement& min, const ArrayElement& max);

    void gen_array();
    SortStats<ArrayElement> get_stats() const;
};
} // namespace SortingBenchmark

#endif