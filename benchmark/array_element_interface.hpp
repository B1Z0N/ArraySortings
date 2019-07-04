#ifndef AOS_LAB2_ARRAY_ELEMENT_INTERFACE
#define AOS_LAB2_ARRAY_ELEMENT_INTERFACE

#include <utility>

namespace ArrEl
{

using ulong = unsigned long;

template <typename T>
class ArrayElement
// class that allows comparison and assignment counting
{
    T elem {};

    static ulong comparisons; // 0 by default
    static ulong assignments; // 0 by default

    static bool cmp_on;  // true by default
    static bool asgn_on; // true by default
public:
    ArrayElement();
    ArrayElement(const T &);
    ArrayElement(const ArrayElement &) = default;
    ArrayElement(T &&);
    ArrayElement(ArrayElement &&) = default;

    ArrayElement &operator=(const ArrayElement &);
    ArrayElement &operator=(const T &);
    ArrayElement &operator=(ArrayElement &&);
    ArrayElement &operator=(T &&);

    static void reset();
    
    static ulong get_cmp();
    static ulong get_asgn();

    static void off_on_cmp_count(bool);
    static void off_on_asgn_count(bool);

    bool operator==(const ArrayElement& other);
    bool operator!=(const ArrayElement& other);
    bool operator>(const ArrayElement& other);
    bool operator<(const ArrayElement& other);
    bool operator>=(const ArrayElement& other);
    bool operator<=(const ArrayElement& other);
};
} // namespace ArrEl
#endif
