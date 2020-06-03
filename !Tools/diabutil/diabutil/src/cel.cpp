#include <diabutil/cel.hpp>

#include <algorithm>
#include <stdexcept>

namespace
{
	constexpr auto transparent_pixel = color_t{0, 0, 0, 0};

	image_t flip_vertically(image_t const &image)
	{
		image_t builder;
		builder.width = image.width;
		builder.height = image.height;

		// TODO: Does std::transform serve us better here?
		for (auto row = builder.height - 1; row >= 0; --row)
		{
			builder.pixels.insert(end(builder.pixels), next(begin(image.pixels), row * image.width), next(begin(image.pixels), (row + 1) * image.width));
		}

		return builder;
	}

} // namespace

std::vector<uint8_t> rle_decode(std::vector<uint8_t> const &rle_cel, bool has_header)
{
	if (rle_cel.empty())
	{
		throw std::invalid_argument{"expected data, got none"};
	}

	std::vector<uint8_t> decoded;

	// Headers are 10 bytes, in that case ignore them
	// TODO: What is in a header?
	size_t const start_index = has_header ? 10 : 0;

	// Decode RLE into raw pixels so the image is easier to manipulate
	for (size_t i = start_index; i < rle_cel.size(); /*loop handles increment*/)
	{
		uint8_t run_width = rle_cel[i];
		++i;

		auto const transparent = (run_width & 0x80) > 0;
		if (transparent)
		{
			auto trans_run_width = -(char)run_width;

			// Need to fill the buffer with pixels
			for (auto runi = 0; runi < trans_run_width; ++runi)
			{
				decoded.push_back(0);
			}
		}
		else
		{
			// Palettize each pixel in a run
			for (int runi = 0; runi < run_width; ++runi)
			{
				auto const pixel = rle_cel[i + runi];
				decoded.push_back(pixel);
			}

			i += run_width;
		}
	}

	return decoded;
}

std::vector<color_t> colorize(std::vector<uint8_t> const &palettized, palette_t const &palette)
{
	if (palette.size() != palette_num_colors)
	{
		throw std::invalid_argument{"palette is not of the right size"};
	}

	std::vector<color_t> colorized;
	colorized.reserve(palettized.size());
	std::transform(begin(palettized), end(palettized), std::back_inserter(colorized), [&palette](uint8_t const &p) {
		if (p == 0)
		{
			return transparent_pixel;
		}
		return palette[p];
	});
	return colorized;
}

image_t image_from_cel(std::vector<uint8_t> const &cel, int width, palette_t const &palette, bool has_header)
{
	if (cel.empty())
	{
		throw std::invalid_argument{"expected data, got none"};
	}
	if (width <= 0)
	{
		throw std::invalid_argument{"expected width > 0"};
	}
	if (palette.size() != palette_num_colors)
	{
		throw std::invalid_argument{"palette is not of the right size"};
	}

	image_t builder;
	builder.pixels = colorize(rle_decode(cel, has_header), palette);
	builder.width = width;
	// TODO: Warning or exception if there's a remainder (means we missed data or don't have enough)
	builder.height = builder.pixels.size() / builder.width;

	return flip_vertically(builder);
}
