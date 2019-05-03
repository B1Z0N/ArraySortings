#ifndef AOS_LAB2_SORT_BENCH_INTERFACE
// Make sorting for 2 types
// Make preconditions
// Finish SortBench

// what about using?
#define AOS_LAB2_SORT_BENCH_INTERFACE

#include <tuple>  // std::tuple
#include <vector> // std::vector
#include <chrono> // time measuring

//#include "array_element_interface.hpp"

namespace SortingBenchmark
{
using uint = unsigned int;

template <typename AElement>
class GenFunc
// class for supporting 2 types of generating functions:
// with const refs arguments and with ordinary arguments
// allows calling function not even thinking about what it's type
{
    // pointer to function, that generates elements in range (min, max)
    using genf = AElement (*)(AElement min, AElement max);

    // pointer to function, that does the same
    // but passing const refs, because of cheapness of this operation
    using const_genf = AElement (*)(const AElement &, const AElement &);

    bool type; // 0 - first(gf), 1 - second(cgf)
    union {
        genf gf;
        const_genf cgf;
    };

    AElement min;
    AElement max;

public:
    GenFunc(genf genf, AElement _min, AElement _max);                     // constructor for 1st type
    GenFunc(const_genf genf, const AElement &_min, const AElement &_max); // constructor for 2nd type

    AElement get_min() const;
    AElement get_max() const;

    void set_min(const AElement &);
    void set_max(const AElement &);

    AElement generate();
};

template <template <typename> typename Array>
class SortFunc
{
public:
    template <typename T>
    virtual void sort(Array<T> &) = 0;
}

template <typename AElement>
struct SortStats
{
    struct MinMax
    {
        AElement min{};
        AElement max{};
    };
    struct Time
    {
        bool done = false;
        std::chrono::duration dur{}; // time in milliseconds
    } struct CmpAsgn
    {
        bool done = false;
        uint comparisons = 0;
        uint assignments = 0;
    };

    Time time;
    MinMax bounds;
    CmpAsgn cmp_asgn;
    uint len = -1;
};
template <typename AElement>
std::ostream &operator<<(std::ostream &, const SortStats<AElement>::MinMax &);
template <typename AElement>
std::ostream &operator<<(std::ostream &, const SortStats<AElement>::CmpAsgn &);
template <typename AElement>
std::ostream &operator<<(std::ostream &, const SortStats<AElement> &);


template <typename AElement>
double dur_to_milliseconds(const SortStats < AElement &);

template <template <typename> typename Array, typename AElement>
class SortBench
// implements time testing
// implements comparison and assignment counting
{
    // using sortf = void (*)(Array<AElement> &);
    // using sort_cmp_asgn = void (*)(Array<ArrayElement<AElement>> &);
    using genf = AElement (*)(AElement min, AElement max);
    using const_genf = AElement (*)(const AElement &, const AElement &);
    // using SortFunc<Array, AElement>::sortf;
    // using GenFunc<AElement>::genf; ??????????????????????
    // using GenFunc<AElement>::const_genf;

    SortFunc<Array> sorting;
    // SortFunc<Array, ArrayElement<AElement>> cmp_asgn_count_sorting;
    GenFunc<AElement> generating;
    SortStats<AElement> stats;
    Array<AElement> array;

    bool full_stats;

protected:
public:
    // SortBench() = delete;
    SortBench(SortFunc<Array> *, genf _gen, AElement _min, AElement _max);
    SortBench(SortFunc<Array> *, const_genf _gen, const AElement &_min, const AElement &_max);
    SortBench(SortFunc<Array> *, const GenFunc<AElement> &);

    void gen_array(uint len);

    SortStats<AElement>::CmpAsgn cmp_asgn_test();
    std::chrono::duration time_test();

    SortStats<AElement> get_stats() const;

    void set_min(const AElement &_min);
    void set_max(const AElement &_max);
    SortStats<AElement>::MinMax get_min_max() const;
};

template <template <typename> typename Array, typename AElement>
SortStats<AElement> full_test(SortBench<Array, AElement> &);

} // namespace SortingBenchmark

#endif