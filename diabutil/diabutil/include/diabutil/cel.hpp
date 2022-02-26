#pragma once

#include <cinttypes>
#include <diabutil/image.hpp>
#include <diabutil/palette.hpp>
#include <diabutil/types.hpp>
#include <vector>

namespace diabutil {

/// Color used to represent transparency (index-color 0)
inline constexpr auto transparent_pixel =
    color_t{.r = 0, .g = 0, .b = 0, .a = 0};

/// Interpret the group table to carve up a grouped .CEL file into individual
/// .CELs
///
/// @param cel A loaded .CEL file with a group table.
/// @param num_groups Number of groups in the file. Typically 8 works.
/// @returns A list of CEL data, with frame headers. If empty then there was
/// probably an error.
/// @remark Unfortunately, the group table and the frame table are slightly
/// different so we can't just use the same function for both
std::vector<byte_span> find_groups(byte_span cel, size_t num_groups);

/// Interpret the frame table to carve up a .CEL file into frames
///
/// @param cel A loaded .CEL file with a frame table.
/// @returns A list of frame data, still RLE encoded, possibly with header. If
/// empty then there was probably an error.
/// @remarks works on both .CEL and .CL2
std::vector<byte_span> find_frames(byte_span cel);

/// Convert a compressed, indexed-color frame into an uncompressed RGB frame.
///
/// Both decompress + colorize happen in one method to properly account for
/// difference of pixel 0 vs transparency and how they can't be represented in a
/// single uint8_t (need 257 distinct values!)
///
/// @param frame The .CEL frame (no frame table, no header)
/// @param palette The translation from indexed-color to RGB
/// @returns Decompressed RGB frame on success, empty on error
/// @remarks Only works on .CEL
std::vector<color_t> colorize_encoded_cel_frame(span<std::byte> frame,
                                                palette_t const &palette);

/// @see colorize_encoded_cel_frame
/// @remarks Only works on .CL2
std::vector<color_t> colorize_encoded_cl2_frame(span<std::byte> frame,
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