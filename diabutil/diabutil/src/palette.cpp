#include <diabutil/palette.hpp>
#include <stdexcept>
#include <string>

namespace diabutil {

std::optional<palette_t> palette_from_data(span<std::byte> data) {
  if (data.size != palette_expected_size) {
    return std::nullopt;
  }

  palette_t palette;
  for (size_t i = 0; i < palette_num_colors; ++i) {
    palette[i] = color_t{
        static_cast<uint8_t>(data.data[i * 3]),
        static_cast<uint8_t>(data.data[i * 3 + 1]),
        static_cast<uint8_t>(data.data[i * 3 + 2]),
        255,
    };
  }

  return palette;
}

std::unordered_map<color_t, uint8_t> generate_palette_inverse(
    palette_t const& palette) {
  auto inverse = std::unordered_map<color_t, uint8_t>{};
  for (size_t i = 0; i < palette.size(); ++i) {
    // TODO: handle collisions?
    inverse[palette[i]] = i;
  }
  return inverse;
}

}  // namespace diabutil