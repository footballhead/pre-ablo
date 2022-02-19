#include <dr_pcx.h>

#include <array>
#include <cstdio>
#include <diabutil/cel.hpp>
#include <diabutil/file.hpp>
#include <diabutil/palette.hpp>
#include <string>
#include <vector>

using namespace std::string_literals;

namespace {
constexpr auto transparent = color_t{0, 255, 0, 255};
}  // namespace

int main(int argc, char** argv) {
  if (argc < 4) {
    fprintf(stderr, "Usage: %s font.pcx font.bin message\n", argv[0]);
    return 1;
  }

  auto const in_pcx_file = argv[1];
  auto const in_kern_file = argv[2];
  auto const message = std::string{argv[3]};

  //
  // Load kerning
  //

  auto kerning_data = read_entire_file(in_kern_file);

  // Load header data then remove it to use array 1:1 with ASCII
  auto space_width = kerning_data.at(0);
  auto space_height = kerning_data.at(1);
  kerning_data.erase(begin(kerning_data), std::next(begin(kerning_data), 2));

  // Space kerning not set properly?
  kerning_data[' '] = space_width;

  //
  // Read PCX
  //

  int pcx_width = 0;
  int pcx_height = 0;
  int pcx_components = 0;
  drpcx_uint8* pcx_data = drpcx_load_file(in_pcx_file, DRPCX_FALSE, &pcx_width,
                                          &pcx_height, &pcx_components, 0);
  if (pcx_data == NULL) {
    fprintf(stderr, "Failed to read: %s\n", in_pcx_file);
    return 1;
  }

  if (pcx_components != 3) {
    drpcx_free(pcx_data);
    fprintf(stderr, "PCX doesn't have 3 components, don't know what to do\n");
    return 1;
  }

  //
  // Turn into image_t
  //

  auto num_frames = pcx_height / space_height;
  if (num_frames != 256) {
    drpcx_free(pcx_data);
    fprintf(stderr, "PCX doesn't have enough characters\n");
    return 1;
  }

  std::vector<image_t> glyphs(num_frames,
                              make_blank_image(pcx_width, space_height));
  for (auto glyph_index = 0; glyph_index < num_frames; ++glyph_index) {
    auto& glyph = glyphs[glyph_index];
    auto const glyph_area = glyph.width * glyph.height;

    auto glpyh_data_base = pcx_data + glyph_area * glyph_index * pcx_components;
    for (auto i = 0; i < glyph_area * pcx_components; i += pcx_components) {
      auto color = color_t{glpyh_data_base[i], glpyh_data_base[i + 1],
                           glpyh_data_base[i + 2], 255};
      if (color == transparent) {
        color = color_t{};
      }
      glyph.set(i / pcx_components, color);
    }
  }

  drpcx_free(pcx_data);

  //
  // Determine width of final image
  //

  auto textWidth = 0;
  for (auto const& c : message) {
    if (c == 0) {
      break;
    }
    textWidth += kerning_data.at(c);
  }

  //
  // Create image and draw text
  //

  auto image = make_blank_image(textWidth, space_height);

  textWidth = 0;
  for (auto const& c : message) {
    if (c == 0) {
      break;
    }
    auto const kern = kerning_data.at(c);
    if (kern == 0) {
      continue;
    }

    auto const& glyph = glyphs.at(c);

    for (int y = 0; y < image.height; ++y) {
      for (int x = 0; x < kern; ++x) {
        auto const color = glyph.at(x, y);
        auto const drawx = x + textWidth;
        auto const drawy = y;
        image.set(drawx, drawy, color);
      }
    }

    textWidth += kern;
  }

  //
  // Save to file
  //

  auto const filename = "drawtextpcx.png";
  if (!save_to_png(image, filename)) {
    fprintf(stderr, "Failed to save to: %s\n", filename);
    return 1;
  }

  return 0;
}
