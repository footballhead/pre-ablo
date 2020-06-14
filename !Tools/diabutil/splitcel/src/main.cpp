#include <diabutil/cel.hpp>
#include <diabutil/file.hpp>

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

using namespace std::string_literals;

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s infile\n", argv[0]);
		fprintf(stderr, "Will create 0.celpart, 1.celpart, ... in the current directory\n");
		return 1;
	}

	auto const contents = read_entire_file(argv[1]);
	auto const split = split_cel(contents);

	for (auto I = begin(split); I != end(split); ++I)
	{
		auto const filename = std::to_string(std::distance(begin(split), I) + 1) + ".celpart";
		std::ofstream outfile{filename, std::ios_base::binary};
		outfile.write(reinterpret_cast<char const *>(I->data()), I->size());
	}

	return 0;
}
