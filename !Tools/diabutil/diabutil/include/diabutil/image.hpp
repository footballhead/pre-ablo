#pragma once

#include <diabutil/color.hpp>

#include <vector>

struct image_t
{
	int width = 0;
	int height = 0;
	std::vector<color_t> pixels;
};
