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
    fprintf(stderr, "Usage: %s file.celpart palette.pal width [--header]\n",
            argv[0]);
    return 1;
  }

  auto const in_cel_file = argv[1];
  auto const in_palette_file = argv[2];
  auto const width = std::stoi(argv[3]);
  auto has_header = false;
  if (argc == 5) {
    has_header = (argv[4] == "--header"s);
  }

  auto const cel_data = read_entire_file(in_cel_file);
  auto const palette_data = read_entire_file(in_palette_file);

  auto const palette = palette_from_data(palette_data);

  auto const image = image_from_cel(cel_data, width, palette, has_header);

  if (image.width * image.height != image.pixels.size()) {
    fprintf(stderr, "WARNING: Image area (%d) != data size (%zu)\n",
            image.width * image.height, image.pixels.size());
  }

  auto const filename = in_cel_file + ".png"s;
  if (!save_to_png(image, filename)) {
    fprintf(stderr, "failed to write %s\n", filename.c_str());
    return 1;
  };

  return 0;
}
