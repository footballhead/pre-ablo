#pragma once

#include <stdexcept>
#include <vector>

namespace diabutil {

/// Custom span type in the absense of C++20. Doubles as an iterator
template <typename T>
struct span {
  T const *data = nullptr;
  size_t size = 0;

  static constexpr span<T> null_span() {
    return {
        nullptr,
        0,
    };
  }

  bool operator==(span<T> const &rhs) const {
    return data == rhs.data && size == rhs.size;
  }

  // Advance the span like an iterator
  span<T> &operator+=(size_t i) {
    if (i > size) {
      throw std::runtime_error{"span +=: i > size"};
    }
    data += i;
    size -= i;
    return *this;
  }

  // Advance the span like an iterator, returning new span iterator
  friend span<T> operator+(span<T> lhs, size_t rhs) {
    lhs += rhs;
    return lhs;
  }

  // Reinterpret some new data from the span
  template <typename ReinterpretT>
  operator ReinterpretT() const {
    if (sizeof(ReinterpretT) > size) {
      throw std::runtime_error{"span: sizeof(ReinterpretT) > size"};
    }
    return *reinterpret_cast<ReinterpretT const *>(data);
  }

  // Treat like a byte iterator and look at front
  T &operator*() const { return *data; }

  // Treat like a byte iterator and move forward one spot
  span<T> &operator++() {
    *this += sizeof(T);
    return *this;
  }
};

template <typename T>
span<T> make_span(std::vector<T> const &vec) {
  return {
      vec.data(),
      vec.size(),
  };
}

}  // namespace diabutil
