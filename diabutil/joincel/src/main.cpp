#include <diabutil/file.hpp>

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

using namespace std::string_literals;

using cel_frame_data = std::vector<uint8_t>;

int main(int argc, char** argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s num_frames outfile.cel\n", argv[0]);
		fprintf(stderr, "Assumes 0.celpart, 1.celpart, ... exist in the current directory\n");
		return 1;
	}

	auto const num_frames = std::atoi(argv[1]);
	auto const out_filename = argv[2];

	//
	// Load frames into memory
	//

	std::vector<cel_frame_data> frame_data;
	frame_data.reserve(num_frames);
	for (int i = 0; i < num_frames; ++i) {
		auto const filename = std::to_string(i + 1) + ".celpart";
		frame_data.emplace_back(read_entire_file(filename));
	}

	std::ofstream out{ out_filename, std::ios_base::binary };

	//
	// Write number of frames
	//

	uint32_t buffer = num_frames;
	out.write(reinterpret_cast<char const*>(&buffer), sizeof(uint32_t));

	//
	// Write frame table
	//

	// total header size
	buffer = (num_frames + 1) * sizeof(uint32_t) + sizeof(uint32_t);
	out.write(reinterpret_cast<char const*>(&buffer), sizeof(uint32_t));

	for (const auto& frame : frame_data) {
		buffer += frame.size();
		out.write(reinterpret_cast<char const*>(&buffer), sizeof(uint32_t));
	}

	//
	// Write each frame
	//

	for (const auto& frame : frame_data) {
		out.write(reinterpret_cast<char const*>(frame.data()), frame.size());
	}

	return 0;
}