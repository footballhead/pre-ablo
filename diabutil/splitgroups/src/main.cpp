#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace std::string_literals;

namespace {
constexpr auto num_groups = 8;

std::vector<uint8_t> read_entire_file(char const *filename) {
  std::ifstream t{filename, std::ios_base::binary};
  if (!t.good()) {
    throw std::runtime_error{"Failed to open file: "s + filename};
  }
  return std::vector<uint8_t>(std::istreambuf_iterator<char>(t),
                              std::istreambuf_iterator<char>());
}

}  // namespace

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s infile.CEL outdir\n", argv[0]);
    fprintf(stderr, "Will create 0.cel, 1.cel, ... in outdir\n");
    return 1;
  }

  auto const infile = argv[1];
  auto const outdir = argv[2];

  //
  // Load data
  //

  auto const contents = read_entire_file(infile);
  auto contents_iter = contents.data();

  //
  // read all group offsets from header
  //

  std::vector<uint32_t> group_offsets;
  group_offsets.reserve(num_groups);
  for (uint32_t i = 0; i < num_groups; ++i) {
    group_offsets.push_back(*reinterpret_cast<uint32_t const *>(contents_iter));
    contents_iter += sizeof(uint32_t);
  }

  //
  // We need to calculate the size for each group
  //

  std::vector<uint32_t> group_sizes;
  group_sizes.reserve(num_groups);
  for (uint32_t i = 1; i < num_groups; ++i) {
    auto const start_offset = group_offsets[i - 1];
    auto const end_offset = group_offsets[i];
    group_sizes.push_back(end_offset - start_offset);
  }

  // Use file size as last end_offset
  group_sizes.push_back(contents.size() - group_offsets.back());

  //
  // Use group sizes to extract each group
  //

  for (uint32_t i = 0; i < num_groups; ++i) {
    auto const filepath =
        std::filesystem::path(outdir) / (std::to_string(i) + ".cel");
    std::ofstream out{filepath.c_str(), std::ios_base::binary};

    auto const size = group_sizes.at(i);
    out.write(reinterpret_cast<char const *>(contents_iter), size);
    contents_iter += size;
  }

  return 0;
}
