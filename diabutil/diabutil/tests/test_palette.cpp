#include <gtest/gtest.h>

#include <diabutil/palette.hpp>
#include <diabutil/span.hpp>
#include <stdexcept>

TEST(palette_from_data_deprecated, no_data_throws_exception) {
  ASSERT_THROW(palette_from_data({}), std::invalid_argument);
}

TEST(palette_from_data_deprecated, all_black_is_successful) {
  // Arrange
  auto const raw_palette = std::vector<uint8_t>(palette_expected_size, 0);
  auto expected_palette = palette_t();
  expected_palette.fill(color_t{.r = 0, .g = 0, .b = 0, .a = 255});

  // Act
  auto const actual_palette = palette_from_data(raw_palette);

  // Assert
  EXPECT_EQ(actual_palette, expected_palette);
}

TEST(palette_from_data, no_data_is_error) {
  auto const palette = diabutil::palette_from_data({});
  ASSERT_EQ(palette, std::nullopt);
}

TEST(palette_from_data, all_black_is_successful) {
  // Arrange
  auto const raw_palette =
      std::vector<std::byte>(palette_expected_size, std::byte{0});
  auto expected_palette = palette_t();
  expected_palette.fill(color_t{.r = 0, .g = 0, .b = 0, .a = 255});

  // Act
  auto const actual_palette =
      diabutil::palette_from_data(diabutil::make_span(raw_palette));

  // Assert
  ASSERT_NE(actual_palette, std::nullopt);
  EXPECT_EQ(*actual_palette, expected_palette);
}