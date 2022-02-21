#include <stb_image_write.h>

#include <cstdio>
#include <diabutil/cel.hpp>
#include <diabutil/file.hpp>
#include <diabutil/palette.hpp>
#include <string>
#include <vector>

using namespace std::string_literals;

int main(int argc, char **argv) {
  if (argc < 4 || argc > 5) {
    fprintf(stderr, "Usage: %s file.celframe palette.pal width [--header]\n",
            argv[0]);
    fprintf(stderr, "File is saved to file.celframe.png\n");
    fprintf(stderr, "This does not work on .CEL files! Use splitcel first!\n");
    return 1;
  }

  auto const in_cel_file = argv[1];
  auto const in_palette_file = argv[2];
  auto const width = std::stoi(argv[3]);
  auto has_header = false;
  // TODO: Add output parameter

  // TODO: Proper argument parsing
  if (argc == 5) {
    has_header = (argv[4] == "--header"s);
  }

  auto const cel_data = diabutil::read_file(in_cel_file);
  if (!cel_data) {
    fprintf(stderr, "Failed to load .CEL: %s\n", in_cel_file);
    return 2;
  }
  auto const palette_data = diabutil::read_file(in_palette_file);
  if (!palette_data) {
    fprintf(stderr, "Failed to load .PAL: %s\n", in_palette_file);
    return 3;
  }

  auto const palette =
      diabutil::palette_from_data(diabutil::make_span(*palette_data));
  if (!palette) {
    fprintf(stderr, "Failed to interpret .PAL: %s\n", in_palette_file);
    return 4;
  }

  // Skip the first 10 bytes (5 words) if the cel has a header
  auto const cel_span = diabutil::span<std::byte>{
      .data = cel_data->data() + (has_header ? 10 : 0),
      .size = cel_data->size() - (has_header ? 10 : 0),
  };

  auto const decompressed = diabutil::decompress_cel_frame(cel_span);
  if (decompressed.empty()) {
    fprintf(stderr, "Failed to decode .CEL: %s\n", in_cel_file);
    return 5;
  }

  auto colorized =
      diabutil::colorize_cel_frame(diabutil::make_span(decompressed), *palette);
  if (colorized.empty()) {
    fprintf(stderr, "Failed to apply palette\n");
    return 6;
  }

  auto const image = diabutil::colorized_to_image(std::move(colorized), width);
  if (!image) {
    fprintf(stderr, "Failed to make usable image\n");
    return 7;
  }

  auto const filename = in_cel_file + ".png"s;
  if (!diabutil::save_to_png(*image, filename.c_str())) {
    fprintf(stderr, "failed to write %s\n", filename.c_str());
    return 8;
  };

  return 0;
}
