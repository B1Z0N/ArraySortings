#ifndef AOS_LAB2_SIMPLE_ARRAY_INTERFACE
#define AOS_LAB2_SIMPLE_ARRAY_INTERFACE

#include <algorithm> // for std::swap
#include <iostream>
#include <initializer_list>

namespace SimArr
{
using uint = unsigned int;

template <typename T>
class SimpleArray
// Class only creates and deletes memory
// for sorting demonstration purposes only
{
    uint len {};
    T *arr;

public:
    class OutOfBoundsIndex  // exception class
    {
    };

    template <typename U>   // copy and swap idiom
    friend void swap(SimpleArray<U> &, SimpleArray<U> &);

    SimpleArray() noexcept;     // constructors
    explicit SimpleArray(uint);
    SimpleArray(T *, uint);
    SimpleArray(std::initializer_list<T>);

    SimpleArray(const SimpleArray<T> &) noexcept; // copy constructor
    SimpleArray(SimpleArray<T> &&) noexcept;      // move constructor

    ~SimpleArray();

    SimpleArray<T> &operator=(const SimpleArray<T> &) noexcept; // copy assignment
    SimpleArray<T> &operator=(SimpleArray<T> &&) noexcept;      // move assignment

    T *get_arr() noexcept;              // get & set
    const T *get_arr() const noexcept;
    uint get_len() const noexcept;

    template <typename U>
    friend std::ostream &operator<<(std::ostream &, const SimpleArray<U> &);    // in & out
    template <typename U>
    friend std::istream &operator>>(std::istream &, SimpleArray<U> &);

    T &operator[](uint);    // access array by index
};
}
#endif