#include <stb_image_write.h>

#include <diabutil/cel.hpp>
#include <diabutil/file.hpp>
#include <diabutil/palette.hpp>

#include <cstdio>
#include <string>
#include <vector>

using namespace std::string_literals;

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s file.celpart palette.pal width\n", argv[0]);
		return 1;
	}

	auto const in_cel_file = argv[1];
	auto const in_palette_file = argv[2];
	auto const width = std::stoi(argv[3]);

	auto const cel_data = read_entire_file(in_cel_file);
	auto const palette_data = read_entire_file(in_palette_file);

	auto const palette = palette_from_data(palette_data);

	auto const image = image_from_cel(cel_data, width, palette, /*has_header=*/true);

	if (image.width * image.height != image.pixels.size())
	{
		fprintf(stderr, "WARNING: Image area (%d) != data size (%zu)\n", image.width * image.height, image.pixels.size());
	}

	auto const filename = in_cel_file + ".png"s;
	if (stbi_write_png(filename.c_str(), image.width, image.height, 4, image.pixels.data(), image.width * sizeof(color_t)) == 0)
	{
		fprintf(stderr, "failed to write %s\n", filename.c_str());
		return 1;
	};

	return 0;
}
