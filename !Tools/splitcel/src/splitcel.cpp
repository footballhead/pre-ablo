#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

using namespace std::string_literals;

std::vector<uint8_t> read_entire_file(char const* filename)
{
	std::ifstream t{ filename, std::ios_base::binary };
	if (!t.good()) {
		throw std::runtime_error{ "Failed to open file: "s + filename };
	}
	return std::vector<uint8_t>(std::istreambuf_iterator<char>(t), std::istreambuf_iterator<char>());
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s infile\n", argv[0]);
		fprintf(stderr, "Will create 0.celpart, 1.celpart, ... in the current directory\n");
		return 1;
	}

	//
	// Load data
	//

	auto const contents = read_entire_file(argv[1]);
	auto contents_iter = contents.data();

	//
	// read number of frames
	//

	auto const num_frames = *reinterpret_cast<uint32_t const*>(contents_iter);
	contents_iter += sizeof(uint32_t);

	//
	// read all frame sizes from frame table
	//

	std::vector<uint32_t> sizes;
	sizes.reserve(num_frames);
	for (uint32_t i = 0; i < num_frames; ++i) {
		auto const start_offset = *reinterpret_cast<uint32_t const*>(contents_iter);
		contents_iter += sizeof(uint32_t);

		auto const end_offset = *reinterpret_cast<uint32_t const*>(contents_iter);		
		// Don't increment because the end of cell N is the start of cel N+1

		auto const size = end_offset - start_offset;
		sizes.push_back(size);
	}

	// Skip past final end_offset to get to data
	contents_iter += sizeof(uint32_t);

	//
	// Use frame sizes to extract each cel
	//

	for (uint32_t i = 0; i < num_frames; ++i) {
		const auto size = sizes.at(i);
		std::ofstream outfile{ std::to_string(i + 1) + ".celpart", std::ios_base::binary };
		outfile.write(reinterpret_cast<char const*>(contents_iter), size);
		contents_iter += size;
	}

	return 0;
}
