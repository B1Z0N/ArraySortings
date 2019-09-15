#ifndef SORTING_BENCH_SORTINGS_HPP
#define SORTING_BENCH_SORTINGS_HPP

#include <functional>
#include <utility>

namespace srtbch {

namespace sortings {

template <typename T>
struct SelectionSort {
  void operator()(T* data, std::size_t size) {
    std::size_t i, j, min_idx;

    for (i = 0; i < size - 1; i++) {
      min_idx = i;
      for (j = i + 1; j < size; j++)
        if (data[j] < data[min_idx]) min_idx = j;

      std::swap(data[min_idx], data[i]);
    }
  }
};

template <typename T>
struct BubbleSort {
  void operator()(T* data, std::size_t size) {
    std::size_t i, j;
    for (i = 0; i < size - 1; i++)
      for (j = 0; j < size - i - 1; j++)
        if (data[j] > data[j + 1]) std::swap(data[j], data[j + 1]);
  }
};

template <typename T>
struct InsertionSort {
  void operator()(T* data, std::size_t size) {
    int i, j, isize = size;
    T* key;

    for (i = 1; i < isize; i++) {
      key = &data[i];
      j = i - 1;

      while (j >= 0 && data[j] > *key) {
        data[j + 1] = data[j];
        j = j - 1;
      }
      data[j + 1] = *key;
    }
  }
};

template <typename T>
struct MergeSort {
  void operator()(T* data, std::size_t size) {}
};

template <typename T>
struct QuickSort {
  void operator()(T* data, std::size_t size) {}
};

template <typename T>
struct HeapSort {
  void operator()(T* data, std::size_t size) {}
};

template <typename T>
struct CountingSort {
  void operator()(T* data, std::size_t size) {}
};

template <typename T>
struct RadixSort {
  void operator()(T* data, std::size_t size) {}
};

template <typename T>
struct BucketSort {
  void operator()(T* data, std::size_t size) {}
};

}  // namespace sortings

}  // namespace srtbch

#endif  // SORTING_BENCH_SORTINGS_HPP