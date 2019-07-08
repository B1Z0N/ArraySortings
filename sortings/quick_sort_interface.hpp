#ifndef AOS_LAB2_QUICK_SORT_INTERFACE

#define AOS_LAB2_QUICK_SORT_INTERFACE

/* C++ implementation QuickSort */


namespace QuickSorting
{
using uint = unsigned int;

template <typename T>
int partition(T *arr, int low, int high);
/* 
   This function takes last element as pivot, places 
   the pivot element at its correct position in sorted 
    array, and places all smaller (smaller than pivot) 
   to left of pivot and all greater elements to right 
   of pivot 
*/

template <typename T>
void _quick_sort_internal(T *arr, int low, int high);
/* 
  The main function that implements QuickSort 
  arr[] --> Array to be sorted, 
  low  --> Starting index, 
  high  --> Ending index 
*/

template <typename T>
void quick_sort(T *arr,  uint low, uint high);
// Function for argument checking 

} // namespace quick_sort
#endif