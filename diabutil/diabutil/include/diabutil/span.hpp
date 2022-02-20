#pragma once

#include <vector>

namespace diabutil {

/// @todo replace with C++20 span
template <typename T>
struct span {
  T const* data = nullptr;
  size_t size = 0;
};

template <typename T>
span<T> make_span(std::vector<T> const& vec) {
  return {.data = vec.data(), .size = vec.size()};
}

}  // namespace diabutil
