#include <stb_image_write.h>

#include <cstdio>
#include <diabutil/cel.hpp>
#include <diabutil/file.hpp>
#include <diabutil/palette.hpp>
#include <string>
#include <vector>

using namespace std::string_literals;

namespace {

image_t flip_vertically(image_t const &image) {
  image_t builder;
  builder.width = image.width;
  builder.height = image.height;
  builder.pixels.reserve(image.width * image.height);

  for (auto row = builder.height - 1; row >= 0; --row) {
    builder.pixels.insert(end(builder.pixels),
                          next(begin(image.pixels), row * image.width),
                          next(begin(image.pixels), (row + 1) * image.width));
  }

  return builder;
}

}  // namespace

int main(int argc, char **argv) {
  if (argc < 4 || argc > 5) {
    fprintf(stderr, "Usage: %s file.png palette.pal out.celframe [--header]\n",
            argv[0]);
    return 1;
  }

  auto const in_png_file = argv[1];
  auto const in_palette_file = argv[2];
  auto const out_file = argv[3];
  auto has_header = false;
  // TODO: Add output parameter

  // TODO: Proper argument parsing
  if (argc == 5) {
    has_header = (argv[4] == "--header"s);
  }

  auto const image = diabutil::load_png(in_png_file);
  if (!image) {
    fprintf(stderr, "Failed to load: %s\n", in_png_file);
    return 2;
  }

  auto const palette_data = diabutil::read_file(in_palette_file);
  if (!palette_data) {
    fprintf(stderr, "Failed to load: %s\n", in_palette_file);
    return 3;
  }

  auto const palette =
      diabutil::palette_from_data(diabutil::make_span(*palette_data));
  if (!palette) {
    fprintf(stderr, "Failed to interpret .PAL: %s\n", in_palette_file);
    return 4;
  }

  auto const palette_inverse = diabutil::generate_palette_inverse(*palette);
  auto const flipped_image = flip_vertically(*image);

  auto cel = std::vector<std::byte>{};

  if (has_header) {
    // TODO: has_header
    // the game will hate you
    cel.push_back(std::byte{0xA});
    cel.push_back(std::byte{0});
    cel.push_back(std::byte{0});
    cel.push_back(std::byte{0});
    cel.push_back(std::byte{0});
    cel.push_back(std::byte{0});
    cel.push_back(std::byte{0});
    cel.push_back(std::byte{0});
    cel.push_back(std::byte{0});
    cel.push_back(std::byte{0});
  }

  // This is the worst way of doing this. It trips up TDG23 but if you correct
  // the dimensions then it looks fine
  for (auto const &pixel : flipped_image.pixels) {
    if (pixel.a == 0) {
      cel.push_back(std::byte{0xFF});  // -1 or 1 transparent pixel
    } else {
      cel.push_back(std::byte{1});                          // copy 1 byte
      cel.push_back(std::byte{palette_inverse.at(pixel)});  // here's the byte
    }
  }

  if (!diabutil::dump_to_disk(diabutil::make_span(cel), out_file)) {
    fprintf(stderr, "Failed to save: %s\n", out_file);
    return 5;
  }

  return 0;
}
