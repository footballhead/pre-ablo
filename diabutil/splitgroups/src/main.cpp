#include <cstdio>
#include <diabutil/cel.hpp>
#include <diabutil/file.hpp>
#include <filesystem>
#include <string>

using namespace std::string_literals;

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s infile.CEL outdir\n", argv[0]);
    fprintf(stderr, "Will create 0.cel, 1.cel, ... in outdir\n");
    return 1;
  }

  auto const infile = argv[1];
  auto const outdir = argv[2];

  auto const contents = diabutil::read_file(infile);
  if (!contents) {
    fprintf(stderr, "Failed to read: %s\n", infile);
    return 2;
  }

  auto const cels = diabutil::split_groups(diabutil::make_span(*contents));
  if (cels.empty()) {
    fprintf(stderr, "Failed to split groups: %s\n", infile);
    return 3;
  }

  for (size_t i = 0; i < cels.size(); ++i) {
    auto const filename = std::to_string(i) + ".cel";
    auto const filepath = std::filesystem::path(outdir) / filename;
    auto const &cel = cels.at(i);

    if (!diabutil::dump_to_disk(diabutil::make_span(cel), filepath.c_str())) {
      fprintf(stderr, "Failed to save frame: %zu\n", i);
      return 4;
    }
  }

  return 0;
}
