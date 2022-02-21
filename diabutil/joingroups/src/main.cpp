#include <cstdio>
#include <diabutil/file.hpp>
#include <fstream>
#include <string>
#include <vector>

using namespace std::string_literals;

namespace {

using cel_data = std::vector<uint8_t>;

}  // namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s num_groups output.cel\n", argv[0]);
    fprintf(stderr,
            "Assumes 0.cel, 1.cel, ... exists in the current directory\n");
    return 1;
  }

  auto const num_groups = std::atoi(argv[1]);
  auto const out_filename = argv[2];

  //
  // Load CELs into memory
  //

  std::vector<cel_data> cels;
  cels.reserve(num_groups);
  for (int i = 0; i < num_groups; ++i) {
    auto const filename = std::to_string(i) + ".cel";
    cels.emplace_back(read_entire_file(filename.c_str()));
  }

  //
  // Write CEL offsets
  //

  std::ofstream out{out_filename, std::ios_base::binary};

  uint32_t buffer = num_groups * sizeof(uint32_t);

  for (const auto& cel : cels) {
    out.write(reinterpret_cast<char const*>(&buffer), sizeof(uint32_t));
    buffer += cel.size();
  }

  //
  // Write each CEL
  //

  for (const auto& cel : cels) {
    out.write(reinterpret_cast<char const*>(cel.data()), cel.size());
  }

  return 0;
}
