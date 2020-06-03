#include <diabutil/palette.hpp>

#include <stdexcept>

palette_t palette_from_data(std::vector<uint8_t> const &data)
{
	palette_t palette;

	if (data.size() != palette_expected_size)
	{
		throw std::invalid_argument{"Expected palette size '" + std::to_string(palette_expected_size) + "', got " + std::to_string(data.size())};
	}

	for (size_t i = 0; i < palette_num_colors; ++i)
	{
		palette[i] = {data[i * 3], data[i * 3 + 1], data[i * 3 + 2], 255};
	}

	return palette;
}
