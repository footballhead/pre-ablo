#include <gtest/gtest.h>

#include <diabutil/palette.hpp>
#include <stdexcept>

TEST(palette_from_data, no_data_throws_exception) {
  ASSERT_THROW(palette_from_data({}), std::invalid_argument);
}

TEST(palette_from_data, all_black_is_successful) {
  // Arrange
  auto const raw_palette = std::vector<uint8_t>(palette_expected_size, 0);
  auto expected_palette = palette_t();
  expected_palette.fill(color_t{.r = 0, .g = 0, .b = 0, .a = 255});

  // Act
  auto const actual_palette = palette_from_data(raw_palette);

  // Assert
  EXPECT_EQ(actual_palette, expected_palette);
}
