#ifndef AOS_LAB2_QUICK_SORT

#define AOS_LAB2_QUICK_SORT

#include "quick_sort_interface.hpp"

namespace QuickSorting
{
template <typename T>
int partition(T *arr, int low, int high)
{
    T pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            i++; // increment index of smaller element
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

template <typename T>
void _quick_sort_internal(T *arr, int low, int high)
{
    if (low < high)
    {
        /* 
           pi is partitioning index, arr[p] is now 
           at right place 
        */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        _quick_sort_internal(arr, low, pi - 1 > 0 ? pi - 1 : 0);
        _quick_sort_internal(arr, pi + 1, high);
    }
}

template <typename T>
void quick_sort(T *arr,  uint low, uint high)
{
    _quick_sort_internal(arr, static_cast<int>(low), static_cast<int>(high));
}

} // namespace quick_sort

#endif