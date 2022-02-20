#include <diabutil/palette.hpp>
#include <stdexcept>
#include <string>

palette_t palette_from_data(std::vector<uint8_t> const &data) {
  palette_t palette;

  if (data.size() != palette_expected_size) {
    throw std::invalid_argument{"Expected palette size '" +
                                std::to_string(palette_expected_size) +
                                "', got " + std::to_string(data.size())};
  }

  for (size_t i = 0; i < palette_num_colors; ++i) {
    palette[i] = {data[i * 3], data[i * 3 + 1], data[i * 3 + 2], 255};
  }

  return palette;
}

namespace diabutil {

std::optional<palette_t> palette_from_data(span<std::byte> data) {
  if (data.size != palette_expected_size) {
    return std::nullopt;
  }

  palette_t palette;
  for (size_t i = 0; i < palette_num_colors; ++i) {
    palette[i] = color_t{
        .r = static_cast<uint8_t>(data.data[i * 3]),
        .g = static_cast<uint8_t>(data.data[i * 3 + 1]),
        .b = static_cast<uint8_t>(data.data[i * 3 + 2]),
        .a = 255,
    };
  }

  return palette;
}

}  // namespace diabutil