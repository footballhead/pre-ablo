#pragma once

#include <cinttypes>
#include <diabutil/image.hpp>
#include <diabutil/palette.hpp>
#include <vector>

// TODO: Provide default palette

/// Undo RLE encoding.
/// @return Palettized image, each byte is a pixel.
/// @see colorize
std::vector<uint8_t> rle_decode(std::vector<uint8_t> const &rle_cel,
                                bool has_header = false);

std::vector<color_t> colorize(std::vector<uint8_t> const &palettized,
                              palette_t const &palette);

image_t image_from_cel(std::vector<uint8_t> const &cel, int width,
                       palette_t const &palette, bool has_header = false);

using cel_data = std::vector<uint8_t>;
/// Given a cel file with a frame table, use the frame table to return the
/// individual cel parts.
std::vector<cel_data> split_cel(std::vector<uint8_t> const &cel);
