#ifndef SORTING_BENCH_SORTINGS_HPP
#define SORTING_BENCH_SORTINGS_HPP

#include <functional>
#include <utility>

namespace srtbch {

namespace sortings {

template <typename T>
struct SelectionSort {
  void operator()(T* data, std::size_t size) const {
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
  void operator()(T* data, std::size_t size) const {
    std::size_t i, j;
    for (i = 0; i < size - 1; i++)
      for (j = 0; j < size - i - 1; j++)
        if (data[j] > data[j + 1]) std::swap(data[j], data[j + 1]);
  }
};

template <typename T>
struct InsertionSort {
  void operator()(T* data, std::size_t size) const {
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
  void operator()(T* data, std::size_t size) const {
    return merge_sort(data, 0, size - 1);
  }

 private:
  void merge(T* data, std::size_t l, std::size_t m, std::size_t r) const {
    std::size_t i, j, k;
    std::size_t n1 = m - l + 1;
    std::size_t n2 = r - m;

    T* L = new T[n1];
    T* R = new T[n2];

    for (i = 0; i < n1; i++) L[i] = data[l + i];
    for (j = 0; j < n2; j++) R[j] = data[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
      if (L[i] <= R[j]) {
        data[k] = L[i];
        i++;
      } else {
        data[k] = R[j];
        j++;
      }
      k++;
    }

    while (i < n1) {
      data[k] = L[i];
      i++;
      k++;
    }

    while (j < n2) {
      data[k] = R[j];
      j++;
      k++;
    }

    delete[] L;
    delete[] R;
  }

  void merge_sort(T* data, std::size_t l, std::size_t r) const {
    if (r - l == 0) return;

    if (l < r) {
      std::size_t m = l + (r - l) / 2;

      merge_sort(data, l, m);
      merge_sort(data, m + 1, r);

      merge(data, l, m, r);
    }
  }
};

template <typename T>
struct QuickSort {
  void operator()(T* data, std::size_t size) const {
    return quick_sort(data, 0, size - 1);
  }

  int partition(T* data, int low, int high) const {
    T& pivot = data[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; ++j) {
      if (data[j] < pivot) {
        ++i;
        std::swap(data[i], data[j]);
      }
    }
    std::swap(data[i + 1], data[high]);

    return (i + 1);
  }

  void quick_sort(T* data, int low, int high) const {
    if (low < high) {
      int pi = partition(data, low, high);

      quick_sort(data, low, pi - 1);
      quick_sort(data, pi + 1, high);
    }
  }
};

template <typename T>
struct HeapSort {
  void operator()(T* data, std::size_t size) const {
    return heap_sort(data, size);
  }

 private:
  void heapify(T* data, std::size_t size, std::size_t i) const {
    std::size_t largest = i;
    std::size_t l = 2 * i + 1;
    std::size_t r = 2 * i + 2;

    if (l < size && data[l] > data[largest]) largest = l;

    if (r < size && data[r] > data[largest]) largest = r;

    if (largest != i) {
      std::swap(data[i], data[largest]);
      heapify(data, size, largest);
    }
  }

  void heap_sort(T* data, std::size_t size) const {
    for (int i = size / 2 - 1; i >= 0; --i) heapify(data, size, i);

    for (int i = size - 1; i >= 0; --i) {
      std::swap(data[0], data[i]);
      heapify(data, i, 0);
    }
  }
};

template <typename T>
struct CountingSort {
  void operator()(T* data, std::size_t size) const {}
};

template <typename T>
struct RadixSort {
  void operator()(T* data, std::size_t size) const {}
};

template <typename T>
struct BucketSort {
  void operator()(T* data, std::size_t size) const {}
};

}  // namespace sortings

}  // namespace srtbch

#endif