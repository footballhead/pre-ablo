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

/// Interpret the frame table to carve up a .CEL file into frames
///
/// @param cel A loaded .CEL file with a frame table.
/// @returns A list of frame data, still RLE encoded, possibly with header. If
/// empty then there was probably an error.
std::vector<std::vector<std::byte>> split_cel(span<std::byte> cel);

/// Undo RLE compression for a single frame. If it has a 10 byte header, you
/// must remove it first!
///
/// @param frame The .CEL frame (no frame table, no header)
/// @returns Decompressed indexed-color frame on success, empty on error
std::vector<std::byte> decompress_cel_frame(span<std::byte> frame);

/// Convert an uncompressed, indexed-color frame into an uncompressed RGB frame.
///
/// @param indexed The .CEL frame (no frame table, no header) put through
/// decompress_cel_frame()
/// @param palette The translation from indexed-color to RGB
/// @returns Decompressed RGB frame on success, empty on error
/// @remark An index-color of 0 is treated as transparency regardless of the
/// palette.
std::vector<color_t> colorize_cel_frame(span<std::byte> indexed,
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