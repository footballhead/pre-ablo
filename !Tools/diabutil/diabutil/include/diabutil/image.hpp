#pragma once

#include <diabutil/color.hpp>

#include <stb_image_write.h>

#include <string>
#include <vector>

struct image_t
{
	int width = 0;
	int height = 0;
	std::vector<color_t> pixels;
};

inline bool save_to_png(image_t const& image, std::string const& filename)
{
	return stbi_write_png(filename.c_str(), image.width, image.height, 4, image.pixels.data(), image.width * sizeof(color_t)) != 0;
}
