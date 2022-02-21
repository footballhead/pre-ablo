#pragma once

#include <cinttypes>
#include <diabutil/image.hpp>
#include <diabutil/palette.hpp>
#include <diabutil/span.hpp>
#include <vector>

// TODO: Provide default palette

/// Undo RLE encoding.
/// @return Palettized image, each byte is a pixel.
/// @see colorize
std::vector<uint8_t> rle_decode(std::vector<uint8_t> const &rle_cel,
                                bool has_header = false);

std::vector<color_t> colorize(std::vector<uint8_t> const &palettized,
                              palette_t const &palette);

/// @deprecated
image_t image_from_cel(std::vector<uint8_t> const &cel, int width,
                       palette_t const &palette, bool has_header = false);

using cel_data = std::vector<uint8_t>;
/// Given a cel file with a frame table, use the frame table to return the
/// individual cel parts.
/// @deprecated
std::vector<cel_data> split_cel(std::vector<uint8_t> const &cel);

namespace diabutil {

/// Color used to represent transparency (index-color 0)
constexpr auto transparent_pixel = color_t{.r = 0, .g = 0, .b = 0, .a = 0};

/// Interpret the group table to carve up a grouped .CEL file into individual
/// .CELs
///
/// @param cel A loaded .CEL file with a group table.
/// @param num_groups Number of groups in the file. Typically 8 works.
/// @returns A list of CEL data, with frame headers. If empty then there was
/// probably an error.
/// @remark Unfortunately, the group table and the frame table are slightly
/// different so we can't just use the same function for both
std::vector<std::vector<std::byte>> split_groups(span<std::byte> cel,
                                                 size_t num_groups = 8);

/// Interpret the frame table to carve up a .CEL file into frames
///
/// @param cel A loaded .CEL file with a frame table.
/// @returns A list of frame data, still RLE encoded, possibly with header. If
/// empty then there was probably an error.
std::vector<std::vector<std::byte>> split_cel(span<std::byte> cel);

/// Convert a compressed, indexed-color frame into an uncompressed RGB frame.
///
/// Both decompress + colorize happen in one method to properly account for
/// difference of pixel 0 vs transparency and how they can't be represented in a
/// single uint8_t (need 257 distinct values!)
///
/// @param frame The .CEL frame (no frame table, no header)
/// @param palette The translation from indexed-color to RGB
/// @returns Decompressed RGB frame on success, empty on error
std::vector<color_t> colorize_encoded_cel_frame(span<std::byte> frame,
                                                palette_t const &palette);

/// Turn a single uncompressed RGB frame into an usable image.
///
/// @param colorized A single RGB frame. Consider std::move()ing if you're done
/// with it!
/// @param width The width of the image
/// @returns A valid image on success, std::nullopt on failure
std::optional<image_t> colorized_to_image(std::vector<color_t> colorized,
                                          int width);

}  // namespace diabutil