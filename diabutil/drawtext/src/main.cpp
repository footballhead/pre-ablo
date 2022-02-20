#include <stb_image_write.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <diabutil/cel.hpp>
#include <diabutil/file.hpp>
#include <diabutil/palette.hpp>
#include <iterator>
#include <string>
#include <vector>

using namespace std::string_literals;

namespace {

// values straight from gmenu.cpp

// Each character is at least KERNSPACE big
constexpr auto KERNSPACE = 2;
// Mapping of ASCII character to Data/BigTGold.CEL frame#
constexpr auto mfonttrans = std::array<uint8_t, 128>{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 0-15
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 16-31
    0,  37, 49, 38, 0,  39, 40, 47, 42, 43, 41, 45, 52, 44, 53, 55,  // 32-47
    36, 27, 28, 29, 30, 31, 32, 33, 34, 35, 51, 50, 0,  46, 0,  54,  // 48-63
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,  // 64-79
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 42, 0,  43, 0,  0,   // 80-95
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,  // 96-111
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 20, 0,  21, 0,  0    // 112-127
};
// Mapping of Data/BigTGold.CEL frame# to desired glyph width
constexpr auto mfontkern = std::array<uint8_t, 56>{
    18,  // Space/Invalid
    33, 21, 26, 28, 19, 19, 26, 25, 11, 12, 25, 19, 34, 28, 32, 20,  // a-p
    32, 28, 20, 28, 36, 35, 46, 33, 33, 24,                          // q-z
    11, 23, 22, 22, 21, 22, 21, 21, 21, 32,                          // 1-0
    10, 20, 36, 31, 17, 13, 12, 13, 18, 16, 11, 20, 21, 11, 10, 12,
    11, 21, 23  // misc
};

constexpr auto BigTGold_width = 46;
constexpr auto BigTGold_height = 45;

}  // namespace

// TODO: Make this a library function???
int main(int argc, char** argv) {
  if (argc < 5) {
    fprintf(stderr, "Usage: %s BigTGold.CEL palette.pal output.png message\n",
            argv[0]);
    fprintf(stderr, "Only works for BigTGold.CEL\n");
    return 1;
  }

  auto const in_cel_file = argv[1];
  auto const in_palette_file = argv[2];
  auto const out_file = argv[3];
  auto const message = std::string{argv[4]};

  //
  // Load palette
  //

  auto const palette_data = diabutil::read_file(in_palette_file);
  if (!palette_data) {
    fprintf(stderr, "Failed to load: %s\n", in_palette_file);
    return 2;
  }
  auto const palette =
      diabutil::palette_from_data(diabutil::make_span(*palette_data));
  if (!palette) {
    fprintf(stderr, "Palette conversion failed: %s\n", in_palette_file);
    return 3;
  }

  //
  // Load BigTGold.cel, turn into frames
  //

  auto const cel_contents = diabutil::read_file(in_cel_file);
  if (!cel_contents) {
    fprintf(stderr, "Failed to load: %s\n", in_cel_file);
    return 4;
  }
  auto const frames = split_cel(diabutil::make_span(*cel_contents));
  if (frames.empty()) {
    fprintf(stderr, "Failed to split: %s\n", in_cel_file);
    return 5;
  }

  // +1 for the space char
  if (frames.size() + 1 != mfontkern.size()) {
    fprintf(stderr, "Expected %zu characters, got %zu\n", mfontkern.size(),
            frames.size());
    return 6;
  }

  //
  // Turn frames into images
  //

  std::vector<image_t> glyphs{};
  glyphs.reserve(frames.size());
  for (auto const& frame : frames) {
    auto const decompressed = diabutil::decompress_cel_frame(diabutil::make_span(frame));
    if (decompressed.empty()) {
      fprintf(stderr, "Failed to decompress frame\n");
      return 7;
    }

    auto colorized = diabutil::colorize_cel_frame(diabutil::make_span(decompressed), *palette);
    if (colorized.empty()) {
      fprintf(stderr, "Failed to colorize frame\n");
      return 8;
    }

    auto image = diabutil::colorized_to_image(std::move(colorized), BigTGold_width);
    if (!image) {
      fprintf(stderr, "Failed to convert into RGB image\n");
      return 9;
    }

    glyphs.push_back(*image);
  }

  //
  // Determine width of final image
  //

  auto textWidth = 0;
  for (auto const& c : message) {
    if (c == 0) {
      break;
    }
    auto const frame = mfonttrans[c];
    textWidth += mfontkern[frame] + KERNSPACE;
  }

  //
  // Create image and draw text
  //

  auto image = make_blank_image(textWidth, BigTGold_height);

  textWidth = 0;
  for (auto const& c : message) {
    if (c == 0) {
      break;
    }

    auto const frame = mfonttrans[c];
    auto const kern = mfontkern[frame];

    if (frame > 0) {
      for (int y = 0; y < BigTGold_height; ++y) {
        for (int x = 0; x < kern; ++x) {
          // -1 because Diablo thinks frames start at 1
          auto const color = glyphs.at(frame - 1).at(x, y);
          auto const drawx = x + textWidth;
          auto const drawy = y;
          image.set(drawx, drawy, color);
        }
      }
    }

    textWidth += mfontkern[frame] + KERNSPACE;
  }

  //
  // Save to file
  //

  if (!save_to_png(image, out_file)) {
    fprintf(stderr, "Failed to save to: %s\n", out_file);
    return 10;
  }

  return 0;
}
