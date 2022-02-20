#include <gtest/gtest.h>

#include <diabutil/color.hpp>

TEST(color_t, matching_colors_are_equal) {
  auto const actual_color = color_t{.r = 0, .g = 1, .b = 2, .a = 3};
  auto const expected_color = color_t{.r = 0, .g = 1, .b = 2, .a = 3};
  // Be explicit, we're testing operator==
  ASSERT_TRUE(actual_color == expected_color);
}

// TODO: Consider parameterized test
TEST(color_t, different_colors_not_equal) {
  auto const actual_color = color_t{.r = 99, .g = 1, .b = 2, .a = 3};
  auto const expected_color = color_t{.r = 0, .g = 1, .b = 2, .a = 3};
  // Be explicit, we're testing operator==
  ASSERT_FALSE(actual_color == expected_color);
}
