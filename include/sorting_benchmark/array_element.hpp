/** @file
template class declaration for comparisons and assignments counting
*/

#ifndef SORT_BENCH_ARRAY_ELEMENT_HPP
#define SORT_BENCH_ARRAY_ELEMENT_HPP

#include <utility>

namespace srtbch {

/** class for comparison and assignment counting */

template <typename T>
class ArrayElement {
  T elem_{};

  static std::size_t comparisons;  ///< 0 by default
  static std::size_t assignments;  ///< 0 by default

  static bool cmp_on;   ///< true by default
  static bool asgn_on;  ///< true by default
 public:
  ArrayElement() = default;
  ArrayElement(const ArrayElement &) = default;
  ArrayElement(ArrayElement &&) = default;
  ArrayElement(T &&);
  ArrayElement(const T &);

  ArrayElement &operator=(const ArrayElement &);
  ArrayElement &operator=(const T &);
  ArrayElement &operator=(ArrayElement &&);
  ArrayElement &operator=(T &&);

  static void reset() noexcept;

  static std::size_t get_cmp() noexcept;
  static std::size_t get_asgn() noexcept;

  static void cmp_count_switch(bool) noexcept;
  static void asgn_count_switch(bool) noexcept;

  bool operator==(const ArrayElement &other) const;
  bool operator!=(const ArrayElement &other) const;
  bool operator>(const ArrayElement &other) const;
  bool operator<(const ArrayElement &other) const;
  bool operator>=(const ArrayElement &other) const;
  bool operator<=(const ArrayElement &other) const;
};

template <typename T>
std::size_t ArrayElement<T>::comparisons = 0;

template <typename T>
std::size_t ArrayElement<T>::assignments = 0;

template <typename T>
bool ArrayElement<T>::cmp_on = true;

template <typename T>
bool ArrayElement<T>::asgn_on = true;

template <typename T>
ArrayElement<T>::ArrayElement(const T &el) : elem_{el} {}

template <typename T>
ArrayElement<T>::ArrayElement(T &&el) : elem_{std::move(el)} {}

template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(const ArrayElement<T> &el) {
  if (asgn_on) assignments++;
  elem_ = el.elem_;

  return (*this);
}

template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(const T &el) {
  if (asgn_on) assignments++;
  elem_ = el;

  return (*this);
}

template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(ArrayElement<T> &&el) {
  if (asgn_on) assignments++;
  elem_ = std::move(el.elem_);

  return (*this);
}

template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(T &&el) {
  if (asgn_on) assignments++;
  elem_ = std::move(el);

  return (*this);
}

template <typename T>
inline void ArrayElement<T>::reset() noexcept {
  comparisons = 0;
  assignments = 0;
}

template <typename T>
inline std::size_t ArrayElement<T>::get_cmp() noexcept {
  return comparisons;
}

template <typename T>
inline std::size_t ArrayElement<T>::get_asgn() noexcept {
  return assignments;
}

template <typename T>
inline void ArrayElement<T>::cmp_count_switch(bool b) noexcept {
  cmp_on = b;
}

template <typename T>
inline void ArrayElement<T>::asgn_count_switch(bool b) noexcept {
  asgn_on = b;
}

template <typename T>
inline bool ArrayElement<T>::operator==(const ArrayElement<T> &other) const {
  if (cmp_on) comparisons++;
  return elem_ == other.elem_;
}

template <typename T>
inline bool ArrayElement<T>::operator!=(const ArrayElement<T> &other) const {
  if (cmp_on) comparisons++;
  return elem_ != other.elem_;
}

template <typename T>
inline bool ArrayElement<T>::operator>(const ArrayElement<T> &other) const {
  if (cmp_on) comparisons++;
  return elem_ > other.elem_;
}

template <typename T>
inline bool ArrayElement<T>::operator<(const ArrayElement<T> &other) const {
  if (cmp_on) comparisons++;
  return elem_ < other.elem_;
}

template <typename T>
inline bool ArrayElement<T>::operator>=(const ArrayElement<T> &other) const {
  if (cmp_on) comparisons++;
  return elem_ >= other.elem_;
}

template <typename T>
inline bool ArrayElement<T>::operator<=(const ArrayElement<T> &other) const {
  if (cmp_on) comparisons++;
  return elem_ <= other.elem_;
}

}  // namespace srtbch

#endif  // SORT_BENCH_ARRAY_ELEMENT_HPP
