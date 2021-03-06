#pragma once

#include <diabutil/color.hpp>

#include <stb_image_write.h>

#include <stdexcept>
#include <string>
#include <vector>

struct image_t
{
	int width = 0;
	int height = 0;
	std::vector<color_t> pixels;

	color_t at(int x, int y) const
	{
		auto const index = y * width + x;
		if (index < 0 || index >= pixels.size()) {
			return color_t{};
		}
		return pixels.at(index);
	}

	void set(int x, int y, color_t const& color)
	{
		set(y * width + x, color);
	}

	void set(int index, color_t const& color)
	{
		if (index < 0 || index >= width * height) {
			throw std::invalid_argument{"Invalid coords"};
		}
		pixels[index] = color;
	}
};

inline bool save_to_png(image_t const& image, std::string const& filename)
{
	return stbi_write_png(filename.c_str(), image.width, image.height, 4, image.pixels.data(), image.width * sizeof(color_t)) != 0;
}

inline image_t make_blank_image(int width, int height)
{
    return {width, height, std::vector<color_t>(width * height, color_t{})};
}
