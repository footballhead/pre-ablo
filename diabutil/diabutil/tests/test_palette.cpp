#include <gtest/gtest.h>

#include <diabutil/palette.hpp>
#include <diabutil/span.hpp>
#include <stdexcept>

namespace diabutil {
namespace {

TEST(palette_from_data, no_data_is_error) {
  auto const palette = palette_from_data({});
  ASSERT_EQ(palette, std::nullopt);
}

TEST(palette_from_data, all_black_is_successful) {
  // Arrange
  auto const raw_palette =
      std::vector<std::byte>(palette_expected_size, std::byte{0});
  auto expected_palette = palette_t();
  expected_palette.fill(color_t{.r = 0, .g = 0, .b = 0, .a = 255});

  // Act
  auto const actual_palette = palette_from_data(make_span(raw_palette));

  // Assert
  ASSERT_NE(actual_palette, std::nullopt);
  EXPECT_EQ(*actual_palette, expected_palette);
}

TEST(generate_palette_inverse, arbitrary_color_succeeds) {
  // Arrange
  auto palette = palette_t{};
  for (size_t i = 0; i < palette.size(); ++i) {
    palette[i] = color_t{
        .r = static_cast<uint8_t>(i),
        .g = static_cast<uint8_t>(i),
        .b = static_cast<uint8_t>(i),
        .a = static_cast<uint8_t>(i),
    };
  }

  // Act
  auto const inverse = generate_palette_inverse(palette);

  // Assert
  auto const test_inverse = inverse.at(color_t{5, 5, 5, 5});
  EXPECT_EQ(test_inverse, 5);
}

}  // namespace
}  // namespace diabutil