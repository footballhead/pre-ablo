#include <gtest/gtest.h>

#include <diabutil/cel.hpp>

namespace diabutil {
namespace {

TEST(find_frames, empty_buffer_returns_empty) {
  auto const input = byte_vector();
  auto const output = find_frames(make_span(input));
  ASSERT_TRUE(output.empty());
}

TEST(find_frames, only_frame_num_returns_empty) {
  auto const input = std::vector<std::byte>{
      std::byte(1), std::byte(0), std::byte(0), std::byte(0),  // 1 frame
      // no frame table, etc.
  };
  auto const output = find_frames(make_span(input));
  ASSERT_TRUE(output.empty());
}

TEST(find_frames, bad_frame_table_returns_empty) {
  auto const input = byte_vector{
      std::byte(1),   std::byte(0),
      std::byte(0),   std::byte(0),  // 1 frame
      std::byte(0xC), std::byte(0),
      std::byte(0),   std::byte(0),  // starts at 0xC
                                     // mssing frame 1 end, etc.
  };
  auto const output = find_frames(make_span(input));
  ASSERT_TRUE(output.empty());
}

TEST(find_frames, no_frame_data_returns_empty) {
  auto const input = byte_vector{
      std::byte(1),   std::byte(0),
      std::byte(0),   std::byte(0),  // 1 frame
      std::byte(0xC), std::byte(0),
      std::byte(0),   std::byte(0),  // starts at 0xC
      std::byte(0xE), std::byte(0),
      std::byte(0),   std::byte(0),  // ends at 0xE
                                     // missing data
  };
  auto const output = find_frames(make_span(input));
  ASSERT_TRUE(output.empty());
}

TEST(find_frames, simple_cel_decodes_properly) {
  auto const input = byte_vector{
      std::byte(1),    std::byte(0),
      std::byte(0),    std::byte(0),  // 1 frame
      std::byte(0xC),  std::byte(0),
      std::byte(0),    std::byte(0),  // starts at 0xC
      std::byte(0xE),  std::byte(0),
      std::byte(0),    std::byte(0),  // ends at 0xE
      std::byte(1),                   // frame 1 contains a run of size 1
      std::byte(0xFF),                // run contains 255
  };
  auto const expected_frame = byte_vector{
      std::byte(1),     // frame 1 contains a run of size 1
      std::byte(0xFF),  // run contains 255
  };
  auto const output = find_frames(make_span(input));
  ASSERT_EQ(output.size(), 1);
  ASSERT_EQ(output[0].size, expected_frame.size());
}

TEST(find_groups, invalid_groups_returns_error) {
  auto const input = byte_vector{};
  auto const invalid_num_groups = 0;
  auto const output = find_groups(make_span(input), invalid_num_groups);
  ASSERT_TRUE(output.empty());
}

TEST(find_groups, header_too_small_returns_error) {
  auto const input = byte_vector{};
  auto const num_groups = 1;
  auto const output = find_groups(make_span(input), num_groups);
  ASSERT_TRUE(output.empty());
}

TEST(find_groups, no_cel_data_returns_error) {
  auto const input = byte_vector{
      std::byte(4), std::byte(0), std::byte(0),
      std::byte(0),  // 1 group, data at 0x4
                     // missing data
  };
  auto const num_groups = 1;
  auto const output = find_groups(make_span(input), num_groups);
  ASSERT_TRUE(output.empty());
}

TEST(find_groups, one_group_decodes_properly) {
  auto const input = byte_vector{
      std::byte(4),   std::byte(0), std::byte(0),
      std::byte(0),    // 1st group, data at 0x4
      std::byte(128),  // group 1
  };
  auto const num_groups = 1;
  auto const output = find_groups(make_span(input), num_groups);
  ASSERT_EQ(output.size(), 1);
  ASSERT_EQ(output[0].size, 1);
  ASSERT_EQ(output[0].data[0], std::byte(128));
}

TEST(find_groups, two_groups_decodes_properly) {
  // This is technically an invalid cel but we don't care about frame contents
  auto const input = byte_vector{
      std::byte(8), std::byte(0), std::byte(0),
      std::byte(0),  // 1st group data is at 0x8
      std::byte(9), std::byte(0), std::byte(0),
      std::byte(0),  // 2nd group data at at 0x9
      std::byte(0),  // group 1
      std::byte(1),  // group 2
  };
  auto const num_groups = 2;
  auto const output = find_groups(make_span(input), num_groups);
  ASSERT_EQ(output.size(), num_groups);
  ASSERT_EQ(output[0].size, 1);
  ASSERT_EQ(output[0].data[0], std::byte(0));
  ASSERT_EQ(output[1].size, 1);
  ASSERT_EQ(output[1].data[0], std::byte(1));
}

TEST(colorize_encoded_cel_frame, empty_returns_error) {
  auto const unused_palette = palette_t{};
  auto const input = byte_vector{};
  auto const output =
      colorize_encoded_cel_frame(make_span(input), unused_palette);
  ASSERT_TRUE(output.empty());
}

TEST(colorize_encoded_cel_frame, not_enough_pixels_returns_error) {
  auto const unused_palette = palette_t{};
  auto const input = byte_vector{
      std::byte{2}, std::byte{255},  // a run of 2, missing a pixel!
  };
  auto const output =
      colorize_encoded_cel_frame(make_span(input), unused_palette);
  ASSERT_TRUE(output.empty());
}

TEST(colorize_encoded_cel_frame, small_sample_succeeds) {
  auto const input = byte_vector{
      std::byte{1},    std::byte{0},    // 1 pixel of 0
      std::byte{0xFF},                  // 1 pixel of transparency
      std::byte{1},    std::byte{255},  // 1 pixel of 255
  };
  auto const arbitrary_color = color_t{1, 2, 3, 255};
  auto const arbitrary_color2 = color_t{4, 5, 6, 255};
  auto palette = palette_t{};
  palette[0] = arbitrary_color;
  palette[255] = arbitrary_color2;
  auto const expected = std::vector<color_t>{arbitrary_color, transparent_pixel,
                                             arbitrary_color2};

  auto const output = colorize_encoded_cel_frame(make_span(input), palette);

  ASSERT_EQ(output, expected);
}

TEST(colorized_to_image, small_sample_succeeds) {
  auto const arbitrary_color = color_t{1, 2, 3, 255};
  auto const input = std::vector<color_t>{arbitrary_color};

  auto const output = colorized_to_image(input, 1);

  ASSERT_NE(output, std::nullopt);
  EXPECT_EQ(output->width, 1);
  EXPECT_EQ(output->height, 1);
  EXPECT_EQ(output->pixels, input);
}

TEST(colorized_to_image, not_enough_data_fails) {
  auto const arbitrary_color = color_t{1, 2, 3, 255};
  auto const input =
      std::vector<color_t>{arbitrary_color, arbitrary_color, arbitrary_color};

  auto const output = colorized_to_image(input, 2);

  // This should fail because input is 3 pixels, but image is only 2 wide, so we
  // have a half-row

  ASSERT_EQ(output, std::nullopt);
}

}  // namespace
}  // namespace diabutil
