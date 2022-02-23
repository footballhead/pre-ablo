#include <gtest/gtest.h>

#include <diabutil/cel.hpp>

namespace diabutil {
namespace {

// TODO split groups tests

TEST(split_cel, empty_buffer_returns_empty) {
  auto const input = std::vector<std::byte>();
  auto const output = split_cel(make_span(input));
  ASSERT_TRUE(output.empty());
}

TEST(split_cel, only_frame_num_returns_empty) {
  auto const input = std::vector<std::byte>{
      std::byte(1), std::byte(0), std::byte(0), std::byte(0),  // 1 frame
      // no frame table, etc.
  };
  auto const output = split_cel(make_span(input));
  ASSERT_TRUE(output.empty());
}

TEST(split_cel, bad_frame_table_returns_empty) {
  auto const input = std::vector<std::byte>{
      std::byte(1),   std::byte(0),
      std::byte(0),   std::byte(0),  // 1 frame
      std::byte(0xC), std::byte(0),
      std::byte(0),   std::byte(0),  // starts at 0xC
                                     // mssing frame 1 end, etc.
  };
  auto const output = split_cel(make_span(input));
  ASSERT_TRUE(output.empty());
}

TEST(split_cel, no_frame_data_returns_empty) {
  auto const input = std::vector<std::byte>{
      std::byte(1),   std::byte(0),
      std::byte(0),   std::byte(0),  // 1 frame
      std::byte(0xC), std::byte(0),
      std::byte(0),   std::byte(0),  // starts at 0xC
      std::byte(0xE), std::byte(0),
      std::byte(0),   std::byte(0),  // ends at 0xE
                                     // missing data
  };
  auto const output = split_cel(make_span(input));
  ASSERT_TRUE(output.empty());
}

TEST(split_cel, simple_cel_decodes_properly) {
  auto const input = std::vector<std::byte>{
      std::byte(1),    std::byte(0),
      std::byte(0),    std::byte(0),  // 1 frame
      std::byte(0xC),  std::byte(0),
      std::byte(0),    std::byte(0),  // starts at 0xC
      std::byte(0xE),  std::byte(0),
      std::byte(0),    std::byte(0),  // ends at 0xE
      std::byte(1),                   // frame 1 contains a run of size 1
      std::byte(0xFF),                // run contains 255
  };
  auto const expected_frame = std::vector<std::byte>{
      std::byte(1),     // frame 1 contains a run of size 1
      std::byte(0xFF),  // run contains 255
  };
  auto const output = split_cel(make_span(input));
  ASSERT_EQ(output.size(), 1);
  ASSERT_EQ(output[0], expected_frame);
}

TEST(colorize_encoded_cel_frame, empty_returns_error) {
  auto const unused_palette = palette_t{};
  auto const input = std::vector<std::byte>{};
  auto const output =
      colorize_encoded_cel_frame(make_span(input), unused_palette);
  ASSERT_TRUE(output.empty());
}

TEST(colorize_encoded_cel_frame, not_enough_pixels_returns_error) {
  auto const unused_palette = palette_t{};
  auto const input = std::vector<std::byte>{
      std::byte{2}, std::byte{255},  // a run of 2, missing a pixel!
  };
  auto const output =
      colorize_encoded_cel_frame(make_span(input), unused_palette);
  ASSERT_TRUE(output.empty());
}

TEST(colorize_encoded_cel_frame, small_sample_succeeds) {
  auto const input = std::vector<std::byte>{
      std::byte{1},    std::byte{0},    // 1 pixel of 0
      std::byte{0xFF},                  // 1 pixel of transparency
      std::byte{1},    std::byte{255},  // 1 pixel of 255
  };
  auto const arbitrary_color = color_t{.r = 1, .g = 2, .b = 3, .a = 255};
  auto const arbitrary_color2 = color_t{.r = 4, .g = 5, .b = 6, .a = 255};
  auto palette = palette_t{};
  palette[0] = arbitrary_color;
  palette[255] = arbitrary_color2;
  auto const expected = std::vector<color_t>{arbitrary_color, transparent_pixel,
                                             arbitrary_color2};

  auto const output = colorize_encoded_cel_frame(make_span(input), palette);

  ASSERT_EQ(output, expected);
}

TEST(colorized_to_image, small_sample_succeeds) {
  auto const arbitrary_color = color_t{.r = 1, .g = 2, .b = 3, .a = 255};
  auto const input = std::vector<color_t>{arbitrary_color};

  auto const output = colorized_to_image(input, 1);

  ASSERT_NE(output, std::nullopt);
  EXPECT_EQ(output->width, 1);
  EXPECT_EQ(output->height, 1);
  EXPECT_EQ(output->pixels, input);
}

TEST(colorized_to_image, not_enough_data_fails) {
  auto const arbitrary_color = color_t{.r = 1, .g = 2, .b = 3, .a = 255};
  auto const input =
      std::vector<color_t>{arbitrary_color, arbitrary_color, arbitrary_color};

  auto const output = colorized_to_image(input, 2);

  // This should fail because input is 3 pixels, but image is only 2 wide, so we
  // have a half-row

  ASSERT_EQ(output, std::nullopt);
}

}  // namespace
}  // namespace diabutil
