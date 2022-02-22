#include <stb_image.h>

#include <diabutil/image.hpp>

namespace diabutil {
namespace {
constexpr auto desired_channels = 4;
}

std::optional<image_t> load_png(char const* filename) {
  int width = 0;
  int height = 0;
  int channels = 0;
  auto* data =
      stbi_load(filename, &width, &height, &channels, desired_channels);
  if (!data) {
    return std::nullopt;
  }

  auto pixels = std::vector<color_t>{};
  pixels.reserve(width * height);

  for (int i = 0; i < width * height * desired_channels;
       i += desired_channels) {
    pixels.emplace_back(color_t{
        .r = data[i],
        .g = data[i + 1],
        .b = data[i + 2],
        .a = data[i + 3],
    });
  }

  stbi_image_free(data);

  return image_t{
      .width = width,
      .height = height,
      .pixels = std::move(pixels),
  };
}

}  // namespace diabutil
