#pragma once

#include <diabutil/color.hpp>

#include <array>
#include <cinttypes>
#include <vector>

constexpr auto palette_num_colors = 256;
constexpr auto palette_bytes_per_color = 3;
constexpr auto palette_expected_size = palette_num_colors * palette_bytes_per_color;

using palette_t = std::array<color_t, palette_num_colors>;

palette_t palette_from_data(std::vector<uint8_t> const &data);
