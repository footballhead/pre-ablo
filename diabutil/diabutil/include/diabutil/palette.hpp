#pragma once

#include <array>
#include <cinttypes>
#include <cstddef>
#include <diabutil/color.hpp>
#include <diabutil/span.hpp>
#include <optional>
#include <vector>

inline constexpr auto palette_num_colors = 256;
inline constexpr auto palette_bytes_per_color = 3;
inline constexpr auto palette_expected_size =
    palette_num_colors * palette_bytes_per_color;

using palette_t = std::array<color_t, palette_num_colors>;

namespace diabutil {

/// Convert a raw .PAL buffer into a usable RGB palette.
///
/// @param data The raw, in-memory .PAL file. Must be palette_expected_size
/// large.
/// @return On success, a usable palette. On error, std::nullopt.
/// @todo accept span when C++20
std::optional<palette_t> palette_from_data(span<std::byte> data);

}  // namespace diabutil