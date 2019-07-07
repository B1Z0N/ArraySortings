// template class for comparisons and assignments counting

#ifndef ARRAY_ELEMENT_INTERFACE
#define ARRAY_ELEMENT_INTERFACE

#include <utility>

namespace srtbch
{

using size_t = unsigned long;

template <typename T>
class ArrayElement
// class for comparison and assignment counting
{
    T elem {};

    static size_t comparisons; // 0 by default
    static size_t assignments; // 0 by default

    static bool cmp_on;  // true by default
    static bool asgn_on; // true by default
public:
    ArrayElement() = default;
    ArrayElement(const T &);
    ArrayElement(const ArrayElement &) = default;
    ArrayElement(T &&);
    ArrayElement(ArrayElement &&) = default;

    ArrayElement &operator=(const ArrayElement &);
    ArrayElement &operator=(const T &);
    ArrayElement &operator=(ArrayElement &&);
    ArrayElement &operator=(T &&);

    static void reset() noexcept;
    
    static size_t get_cmp() noexcept;
    static size_t get_asgn() noexcept;

    static void off_on_cmp_count(bool) noexcept;
    static void off_on_asgn_count(bool) noexcept;

    bool operator==(const ArrayElement& other);
    bool operator!=(const ArrayElement& other);
    bool operator>(const ArrayElement& other);
    bool operator<(const ArrayElement& other);
    bool operator>=(const ArrayElement& other);
    bool operator<=(const ArrayElement& other);
};
} // namespace ArrEl
#endif
