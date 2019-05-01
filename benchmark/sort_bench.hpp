#ifndef AOS_LAB2_SORT_BENCH
#define AOS_LAB2_SORT_BENCH

#include <tuple>  // std::tuple
#include <vector> // std::vector

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
AElement GenFunc<AElement>::operator()()
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

//////////-SortFunc-//////////////

template <template <typename> typename A, typename AE /*, typename ...PreconditionReturnTypes */>
SortFunc<A, AE>::SortFunc(SortFunc<A, AE>::sortf _sorting) : sorting{_sorting}
{
}

template <template <typename> typename A, typename AE /*, typename ...PreconditionReturnTypes */>
void SortFunc<A, AE>::sort()
{
    if (arr == nullptr)
        throw ArrayNotGiven{};

    sorting(*arr);
}

//////////-ArrayElement-//////////////
template <typename T>
uint ArrayElement<T>::comparisons = 0;
template <typename T>
uint ArrayElement<T>::assignments = 0;

template <typename T>
bool ArrayElement<T>::cmp_on = true;
template <typename T>
bool ArrayElement<T>::asgn_on = true;

template <typename T>
ArrayElement<T>::ArrayElement(const T &el)
    : elem{el}
{
}

template <typename T>
ArrayElement<T>::ArrayElement(T &&el)
    : elem{std::move(el.elem)}
{
}

template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(const ArrayElement<T> &el)
{
    if (asgn_on)
        assignments++;
    elem = el.elem;

    return (*this);
}
template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(const T &el)
{
    if (asgn_on)
        assignments++;
    elem = el;

    return (*this);
}
template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(ArrayElement<T> &&el)
{
    if (asgn_on)
        assignments++;
    elem = std::move(el.elem);

    return (*this);
}
template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(T &&el)
{
    if (asgn_on)
        assignments++;
    elem = std::move(el);

    return (*this);
}

template <typename T>
inline void ArrayElement<T>::reset()
{
    comparisons = 0;
    assignments = 0;
}

template <typename T>
inline bool operator==(const ArrayElement<T> &a, const ArrayElement<T> &b)
{
    if (a.cmp_on)
        a.comparisons++;
    return a.elem == b.elem;
}
template <typename T>
inline bool operator!=(const ArrayElement<T> &a, const ArrayElement<T> &b)
{
    if (a.cmp_on)
        a.comparisons++;
    return a.elem != b.elem;
}
template <typename T>
inline bool operator>(const ArrayElement<T> &a, const ArrayElement<T> &b)
{
    if (a.cmp_on)
        a.comparisons++;
    return a.elem > b.elem;
}
template <typename T>
inline bool operator<(const ArrayElement<T> &a, const ArrayElement<T> &b)
{
    if (a.cmp_on)
        a.comparisons++;
    return a.elem < b.elem;
}
template <typename T>
inline bool operator>=(const ArrayElement<T> &a, const ArrayElement<T> &b)
{
    if (a.cmp_on)
        a.comparisons++;
    return a.elem >= b.elem;
}
template <typename T>
inline bool operator<=(const ArrayElement<T> &a, const ArrayElement<T> &b)
{
    if (a.cmp_on)
        a.comparisons++;
    return a.elem <= b.elem;
}
} // namespace SortingBenchmark

#endif