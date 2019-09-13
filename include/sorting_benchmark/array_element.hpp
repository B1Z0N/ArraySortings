/** @file
template class declaration for comparisons and assignments counting
*/

#ifndef ARRAY_ELEMENT_HPP
#define ARRAY_ELEMENT_HPP

#include <utility>

namespace srtbch {

using size_t = unsigned long;

/** class for comparison and assignment counting */

template <typename T>
class ArrayElement {
  T elem{};

  static size_t comparisons;  ///< 0 by default
  static size_t assignments;  ///< 0 by default

  static bool cmp_on;   ///< true by default
  static bool asgn_on;  ///< true by default
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

  bool operator==(const ArrayElement &other);
  bool operator!=(const ArrayElement &other);
  bool operator>(const ArrayElement &other);
  bool operator<(const ArrayElement &other);
  bool operator>=(const ArrayElement &other);
  bool operator<=(const ArrayElement &other);
};

template <typename T>
size_t ArrayElement<T>::comparisons = 0;

template <typename T>
size_t ArrayElement<T>::assignments = 0;

template <typename T>
bool ArrayElement<T>::cmp_on = true;

template <typename T>
bool ArrayElement<T>::asgn_on = true;

template <typename T>
ArrayElement<T>::ArrayElement(const T &el) : elem{el} {}

template <typename T>
ArrayElement<T>::ArrayElement(T &&el) : elem{std::move(el)} {}

template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(const ArrayElement<T> &el) {
  if (asgn_on) assignments++;
  elem = el.elem;

  return (*this);
}

template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(const T &el) {
  if (asgn_on) assignments++;
  elem = el;

  return (*this);
}

template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(ArrayElement<T> &&el) {
  if (asgn_on) assignments++;
  elem = std::move(el.elem);

  return (*this);
}

template <typename T>
ArrayElement<T> &ArrayElement<T>::operator=(T &&el) {
  if (asgn_on) assignments++;
  elem = std::move(el);

  return (*this);
}

template <typename T>
inline void ArrayElement<T>::reset() noexcept {
  comparisons = 0;
  assignments = 0;
}

template <typename T>
inline size_t ArrayElement<T>::get_cmp() noexcept {
  return comparisons;
}

template <typename T>
inline size_t ArrayElement<T>::get_asgn() noexcept {
  return assignments;
}

template <typename T>
inline void ArrayElement<T>::off_on_cmp_count(bool b) noexcept {
  cmp_on = b;
}

template <typename T>
inline void ArrayElement<T>::off_on_asgn_count(bool b) noexcept {
  asgn_on = b;
}

template <typename T>
inline bool ArrayElement<T>::operator==(const ArrayElement<T> &other) {
  if (cmp_on) comparisons++;
  return elem == other.elem;
}

template <typename T>
inline bool ArrayElement<T>::operator!=(const ArrayElement<T> &other) {
  return !(*this == other);
}

template <typename T>
inline bool ArrayElement<T>::operator>(const ArrayElement<T> &other) {
  if (cmp_on) comparisons++;
  return elem > other.elem;
}

template <typename T>
inline bool ArrayElement<T>::operator<(const ArrayElement<T> &other) {
  return !(*this == other) && !(*this > other);
}

template <typename T>
inline bool ArrayElement<T>::operator>=(const ArrayElement<T> &other) {
  return !(*this < other);
}

template <typename T>
inline bool ArrayElement<T>::operator<=(const ArrayElement<T> &other) {
  return !(*this > other);
}

};  // namespace srtbch

#endif  // ARRAY_ELEMENT_HPP
