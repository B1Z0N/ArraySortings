#ifndef AOS_LAB2_BUBBLE_SORTING
#define AOS_LAB2_BUBBLE_SORTING

#include <algorithm> // for std::swap

namespace BubbleSorting
{
using uint = unsigned int;

template <typename T>
void bubble_sort(T *arr, uint len) noexcept
{
    for (uint i = 0; i < len - 1; i++)
        for (uint j = 0; j < len - i - 1; j++)
            if (arr[j] > arr[j + 1])
                std::swap(arr[j], arr[j + 1]);
}
} // namespace BubbleSorting
#endif