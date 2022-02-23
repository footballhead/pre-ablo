#pragma once

#include <cinttypes>

/// Standard red/green/blue color model representation with an alpha component.
struct color_t {
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t a = 0;
};

// TODO: replace with C++20 default comparisons
constexpr inline bool operator==(color_t const& a, color_t const& b) {
  return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}
