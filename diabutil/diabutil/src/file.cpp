#include <diabutil/file.hpp>

#include <fstream>
#include <stdexcept>

std::vector<uint8_t> read_entire_file(std::string const &filename)
{
	std::ifstream in{filename, std::ios_base::binary};
	if (!in.good())
	{
		throw std::runtime_error{"Failed to open file: " + filename};
	}
	return std::vector<uint8_t>(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

void dump_to_disk(std::vector<uint8_t> const &data, std::string const &filename)
{
	std::ofstream out{filename, std::ios_base::binary};
	if (!out.good())
	{
		throw std::runtime_error{"Failed to open file: " + filename};
	}
	out.write(reinterpret_cast<char const *>(data.data()), data.size());
	if (!out.good())
	{
		throw std::runtime_error{"Failed to write data"};
	}
}