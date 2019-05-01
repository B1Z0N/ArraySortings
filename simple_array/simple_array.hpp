#ifndef AOS_LAB2_SIMPLE_ARRAY
#define AOS_LAB2_SIMPLE_ARRAY

#include <algorithm> // for std::swap
#include <iostream>
#include <initializer_list>

#include "simple_array_interface.hpp"

namespace SimArr
{
template <typename T>
void swap(SimpleArray<T> &a1, SimpleArray<T> &a2)
{
    std::swap(a1.arr, a2.arr);
    std::swap(a1.len, a2.len);
}

template <typename T>
SimpleArray<T>::SimpleArray() noexcept
    : len{0}, arr{nullptr} {}

template <typename T>
SimpleArray<T>::SimpleArray(uint length)
    : len{length}, arr{new T[length]}
{
    for (uint i = 0; i < len; i++)
    {
        arr[i] = T();
    }
}

template <typename T>
SimpleArray<T>::SimpleArray(T *array, uint length)
{
    len = length;
    arr = new T[len];
    for (uint i = 0; i < len; i++)
    {
        arr[i] = array[i];
    }
}

template <typename T>
SimpleArray<T>::SimpleArray(std::initializer_list<T> lst)
    : SimpleArray<T>(lst.begin(), lst.size()) {}

template <typename T>
SimpleArray<T>::SimpleArray(const SimpleArray<T> &array) noexcept // copy constructor
    : len{array.len}, arr{new T[len]}
{
    std::copy(array.arr, array.arr + len - 1, arr);
}

template <typename T>
SimpleArray<T>::SimpleArray(SimpleArray<T> &&array) noexcept // move constructor
    : SimpleArray<T>()
{
    swap((*this), array);
}

template <typename T>
inline SimpleArray<T>::~SimpleArray() { delete[] arr; }

template <typename T>
SimpleArray<T> &SimpleArray<T>::operator=(const SimpleArray<T> &array) noexcept // copy assignment
{
    SimpleArray<T> tmp(array);
    swap((*this), tmp);

    return (*this);
}

template <typename T>
SimpleArray<T> &SimpleArray<T>::operator=(SimpleArray<T> &&array) noexcept // move assignment
{
    SimpleArray<T> tmp(std::move(array));
    swap((*this), tmp);

    return (*this);
}

template <typename T>
inline T *SimpleArray<T>::get_arr() noexcept { return arr; }

template <typename T>
inline const T *SimpleArray<T>::get_arr() const noexcept
{
    const T *temp = arr;
    return temp;
}

template <typename T>
inline uint SimpleArray<T>::get_len() const noexcept { return len; }

template <typename T>
std::ostream &operator<<(std::ostream &os, const SimpleArray<T> &array)
{
    T *arr = array.arr;
    uint len = array.len;

    if (len == 0)
        return os << "[ ]";

    os << "[" << arr[0];
    if (len >= 1)
    {
        for (uint i = 1; i < len; i++)
        {
            os << ", " << arr[i];
        }
    }

    return os << "]";
}

template <typename T>
std::istream &operator>>(std::istream &is, SimpleArray<T> &array)
{
    std::cout << "Enter length:\n";
    is >> array.len;
    std::cout << "Enter elements:\n";

    array.arr = new T[array.len];

    for (uint i = 0; i < array.len; i++)
        is >> array.arr[i];

    return is;
}

template <typename T>
T &SimpleArray<T>::operator[](uint i)
{
    if (i > len - 1)
        throw OutOfBoundsIndex{};

    return arr[i];
}
} // namespace SimArr
#endif