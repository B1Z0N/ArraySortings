#ifndef AOS_LAB2_SORT_BENCH
#define AOS_LAB2_SORT_BENCH

#include <tuple>  // std::tuple
#include <vector> // std::vector
#include <chrono> // time measuring

#include "sort_bench_interface.hpp"

namespace SortingBenchmark
{
//////////-GenFunc-//////////////

template <typename AElement>
GenFunc<AElement>::GenFunc(GenFunc<AElement>::genf genf, AElement _min, AElement _max)
    : type{0}, gf{genf}, min{_min}, max{_max}
{
}

template <typename AElement>
GenFunc<AElement>::GenFunc(GenFunc<AElement>::const_genf genf, const AElement &_min, const AElement &_max)
    : type{1}, cgf{genf}, min{_min}, max{_max}
{
}

template <typename AElement>
inline AElement GenFunc<AElement>::get_min() const
{
    return min;
}

template <typename AElement>
inline AElement GenFunc<AElement>::get_max() const
{
    return max;
}

template <typename AElement>
inline void GenFunc<AElement>::set_min(const AElement &_min)
{
    min = _min;
}

template <typename AElement>
inline void GenFunc<AElement>::set_max(const AElement &_max)
{
    max = _max;
}

template <typename AElement>
AElement GenFunc<AElement>::generate()
{
    switch (type)
    {
    case 0:
        return gf(min, max);
        break;
    case 1:
        return cgf(min, max);
        break;
    }
}
////////////-SortStats-///////////////////
ostream &operator<<(ostream &os, const SortStats<AElement>::MinMax &_time)
{
    if (_time.done)
    {
        os << "time: " << _time.dur;
    }
    else
        os << "Time test was skipped";

    return os;
}

template <typename AElement>
ostream &operator<<(ostream &os, const SortStats<AElement>::CmpAsgn &cmp_asgn)
{
    if (cmp_asgn.done)
    {
        os << "cmp: " << cmp_asgn.comparisons << "; asgn: " << cmp_asgn.assignments;
    }
    else
        os << "Comparison and assignment tests were skipped";

    return os;
}

ostream &operator<<(ostream &os, const SortStats &stats)
{
    if (stats.len != -1)
    {
        os << "len: " << stats.len;
    }
    else
    {
        //-//
        os << "Array was not initialized, no tests runned";
        return os;
    }

    os << ";\n" << stats.cmp_asgn << ";\n" << stats.bounds;

    return os;
}

template <typename AE>
double dur_to_milliseconds(const SortStats<AE> &stats)
{
    std::chrono::duration dur = stats.time.dur;

    double ms_duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();

    return ms_duration;
}

////////////-SortBench-///////////////////

template <template <typename> typename A, typename AE>
SortBench<A, AE>::SortBench(SortFunc<A> *_sort, const GenFunc<AE> &_gen)
    : sorting{_sort}, generating{_gen}
{
    stats.min = _gen.get_min();
    stats.max = _gen.get_max();
}

template <template <typename> typename A, typename AE>
SortBench<A, AE>::SortBench(SortFunc<A> _sort, SortBench<A, AE>::genf _gen, AE _min, AE _max)
    : sorting{_sort}, generating{_gen, _min, _max}
{
    stats.min = _min;
    stats.max = _max;
}

template <template <typename> typename A, typename AE>
SortBench<A, AE>::SortBench(
    SortFunc<A> _sort, sort_cmp_asgn,
    SortBench<A, AE>::const_genf _gen,
    const AE &_min, const AE &_max)
    : sorting{_sort}, generating{_gen, _min, _max}
{
    stats.min = _min;
    stats.max = _max;
}

template <template <typename> typename A, typename AE>
void SortBench<A, AE>::gen_array(uint len)
{
    stats.len = len;
    array = A<AE>(len);
    for (uint i = 0; i < len; i++)
    {
        array[i] = generating();
    }
}

template <template <typename> typename A, typename AE>
SortStats<AElement>::CmpAsgn SortBench<A, AE>::cmp_asgn_test()
{
    A<ArrayElement<AE>> elemarr(stats.len);
    for (int i = 0; i < stats.len; i++)
    {
        elemarr[i] = ArrayElement<AE>(array[i]);
    }

    sorting.sort(elemarr);

    stats.comparisons = ArrayElement<AE>::get_cmp();
    stats.assignments = ArrayElement<AE>::get_asgn();

    return SortStats<AE>::CmpAsgn{stats.comparisons, stats.assignments};
}

template <template <typename> typename A, typename AE>
std::chrono::duration SortBench<A, AE>::time_test()
{
    using _clock = std::chrono::high_resolution_clock;

    std::chrono::time_point<_clock> then = _clock::now();
    sorting.sort(array);
    std::chrono::time_point<_clock> now = _clock::now();

    stats.time = now - then;

    return stats.time;
}

template <template <typename> typename A, typename AE>
SortStats<AE> SortBench<A, AE>::get_stats() const
{
    return stats;
}

template <template <typename> typename A, typename AE>
void SortBench<A, AE>::set_min(const AElement &_min)
{
    generating.set_min(_min);
    stats.bounds.min = min;
}

template <template <typename> typename A, typename AE>
void SortBench<A, AE>::set_max(const AElement &_max)
{
    generating.set_max(_max);
    stats.bounds.max = max;
}

template <template <typename> typename A, typename AE>
SortStats<AElement>::MinMax SortBench<A, AE>::get_min_max() const
{
    return stats.bounds;
}

template <template <typename> typename A, typename AE>
SortStats<AE> full_test(SortBench<A, AE> &bench)
{
    bench.gen_array();
    bench.cmp_asgn_test();
    bench.time_test();

    return bench.get_stats();

}

} // namespace SortingBenchmark

#endif