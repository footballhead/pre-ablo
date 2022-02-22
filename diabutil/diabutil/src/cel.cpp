#include <algorithm>
#include <diabutil/cel.hpp>
#include <diabutil/span.hpp>
#include <iterator>
#include <stdexcept>

namespace diabutil {
namespace {

image_t flip_vertically(image_t const &image) {
  image_t builder;
  builder.width = image.width;
  builder.height = image.height;
  builder.pixels.reserve(image.width * image.height);

  for (auto row = builder.height - 1; row >= 0; --row) {
    builder.pixels.insert(end(builder.pixels),
                          next(begin(image.pixels), row * image.width),
                          next(begin(image.pixels), (row + 1) * image.width));
  }

  return builder;
}

/// Push the data pointer forward while maintaining correct size.
template <typename SpanT>
void span_advance(span<SpanT> &span, size_t n) {
  span.data += n;
  span.size -= n;
}

/// Reinterpret some data from a byte stream. Calls advance() so that get()
/// can be called in succession to interpret a buffer.
///
/// @returns If not enough data in span, std::nullopt. Otherwise, a conversion
/// to DataType.
/// @todo This only works on x86 systems (little-endian, no alignment)
template <typename DataType, typename SpanT>
std::optional<DataType> span_get(span<SpanT> &span) {
  if (span.size < sizeof(DataType)) {
    return std::nullopt;
  }

  auto const value = *reinterpret_cast<DataType const *>(span.data);
  span_advance(span, sizeof(DataType));

  return value;
}

}  // namespace

std::vector<std::vector<std::byte>> split_groups(span<std::byte> cel_span,
                                                 size_t num_groups) {
  if (cel_span.size == 0) {
    return {};
  }

  // Copy size before mutation starts
  auto const total_size = cel_span.size;

  // +1 because final offset is end-of-file (so all sizes can be calculated)
  auto num_offsets = num_groups + 1;
  auto offsets = std::vector<uint32_t>();
  offsets.reserve(num_offsets);

  // Read available group offsets from file
  for (size_t i = 0; i < num_groups; ++i) {
    auto value = span_get<uint32_t>(cel_span);
    if (!value) {
      return {};
    }
    offsets.push_back(*value);
  }

  // EOF offset not included so add ourselves
  offsets.push_back(total_size);

  std::vector<uint32_t> sizes;
  sizes.reserve(num_groups);
  for (size_t i = 1; i < num_offsets; ++i) {
    auto const start_offset = offsets.at(i - 1);
    auto const end_offset = offsets.at(i);
    auto const size = end_offset - start_offset;
    sizes.push_back(size);
  }

  std::vector<std::vector<std::byte>> split;
  split.reserve(num_groups);
  for (size_t i = 0; i < num_groups; ++i) {
    auto const frame_size = sizes.at(i);
    if (frame_size > cel_span.size) {
      return {};
    }

    // construct a vector by copying between two iterators
    split.emplace_back(cel_span.data, cel_span.data + frame_size);
    span_advance(cel_span, frame_size);
  }

  return split;
}

std::vector<std::vector<std::byte>> split_cel(span<std::byte> cel_span) {
  if (cel_span.size == 0) {
    return {};
  }

  auto const num_frames = span_get<uint32_t>(cel_span);
  if (!num_frames) {
    return {};
  }

  // +1 because final offset is end-of-file (so all sizes can be calculated)
  auto num_offsets = *num_frames + 1;
  auto offsets = std::vector<uint32_t>();
  offsets.reserve(num_offsets);
  for (size_t i = 0; i < num_offsets; ++i) {
    auto value = span_get<uint32_t>(cel_span);
    if (!value) {
      return {};
    }
    offsets.push_back(*value);
  }

  std::vector<uint32_t> sizes;
  sizes.reserve(*num_frames);
  for (size_t i = 1; i < num_offsets; ++i) {
    auto const start_offset = offsets.at(i - 1);
    auto const end_offset = offsets.at(i);
    auto const size = end_offset - start_offset;
    sizes.push_back(size);
  }

  std::vector<std::vector<std::byte>> split;
  split.reserve(*num_frames);
  for (size_t i = 0; i < *num_frames; ++i) {
    auto const frame_size = sizes.at(i);
    if (frame_size > cel_span.size) {
      return {};
    }

    // construct a vector by copying between two iterators
    split.emplace_back(cel_span.data, cel_span.data + frame_size);
    span_advance(cel_span, frame_size);
  }

  return split;
}

std::vector<color_t> colorize_encoded_cel_frame(span<std::byte> frame,
                                                palette_t const &palette) {
  if (frame.size == 0) {
    return {};
  }

  std::vector<color_t> colorized;
  for (size_t i = 0; i < frame.size; /*loop handles increment*/) {
    auto const run_width = static_cast<uint8_t>(frame.data[i]);
    ++i;

    auto const transparent = (run_width & 0x80) > 0;
    if (transparent) {
      auto const trans_run_width = -static_cast<int8_t>(run_width);

      // Need to fill the buffer with pixels
      colorized.resize(colorized.size() + trans_run_width, transparent_pixel);
    } else {
      // Consider if not enough pixels
      if (i + run_width > frame.size) {
        return {};
      }

      // Copy each pixel in a run
      for (uint8_t runi = 0; runi < run_width; ++runi) {
        auto const pixel = static_cast<size_t>(frame.data[i + runi]);
        colorized.push_back(palette[pixel]);
      }

      i += run_width;
    }
  }

  return colorized;
}

std::vector<color_t> colorize_encoded_cl2_frame(span<std::byte> frame,
                                                palette_t const &palette) {
  if (frame.size == 0) {
    return {};
  }

  std::vector<color_t> colorized;
  for (size_t i = 0; i < frame.size; /*loop handles increment*/) {
    auto run_width = (char)(frame.data[i]);
    ++i;

    if (run_width == 0) {
      return {};
    }

    // run_width does double duty. It encodes both:
    //
    // (a) mode, whether to (i) fill a single value (ii) copy a span (iii)
    // insert transparency
    //
    // (b) size, how many transparent pixels, how large a copy, etc.

    auto const transparent_mode = run_width >= 0;
    if (transparent_mode) {
      // Need to fill the buffer with pixels
      colorized.resize(colorized.size() + run_width, transparent_pixel);
      continue;
    }

    run_width = -run_width;
    auto const fill_mode = run_width > 65;
    if (fill_mode) {
      run_width -= 65;

      if (i >= frame.size) {
        // Not enough data for operation
        return {};
      }
      auto const fill_pixel = frame.data[i];
      ++i;

      auto const fill_color = palette[static_cast<size_t>(fill_pixel)];
      colorized.resize(colorized.size() + run_width, fill_color);
      continue;
    }

    // not transparency or fill, must be copy!

    if (i + run_width >= frame.size) {
      // Not enough data for operation
      return {};
    }

    for (char copyi = 0; copyi < run_width; ++copyi) {
      auto const copy_pixel = frame.data[i + copyi];
      auto const copy_color = palette[static_cast<size_t>(copy_pixel)];
      colorized.push_back(copy_color);
    }

    i += run_width;
  }

  return colorized;
}

std::optional<image_t> colorized_to_image(std::vector<color_t> colorized,
                                          int width) {
  if (colorized.size() == 0) {
    return std::nullopt;
  }
  if (width <= 0) {
    return std::nullopt;
  }
  if (colorized.size() % width != 0) {
    return std::nullopt;
  }

  image_t builder;
  builder.pixels = std::move(colorized);
  builder.width = width;
  builder.height = builder.pixels.size() / builder.width;

  // The image is stored bottom-to-top. We reconstruct from top-to-bottom so we
  // have to flip
  return flip_vertically(builder);
}

}  // namespace diabutil