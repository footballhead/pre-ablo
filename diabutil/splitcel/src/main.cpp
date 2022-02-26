#include <cstdio>
#include <diabutil/cel.hpp>
#include <diabutil/file.hpp>
#include <filesystem>
#include <string>

using namespace std::string_literals;

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s infile.CEL outdir\n", argv[0]);
    fprintf(stderr,
            "Will create 0.celframe, 1.celframe, ... in the given outdir\n");
    return 1;
  }

  auto const infile = argv[1];
  auto const outdir = argv[2];
  // TODO: Accept prefix/suffix scheme

  if (!std::filesystem::exists(outdir)) {
    if (!std::filesystem::create_directories(outdir)) {
      fprintf(stderr, "Failed to create output directory: %s\n", outdir);
      return 2;
    }
  }

  auto const contents = diabutil::read_file(infile);
  if (!contents) {
    fprintf(stderr, "Failed to read: %s\n", infile);
    return 3;
  }

  auto const frames = diabutil::split_cel(diabutil::make_span(*contents));
  if (frames.empty()) {
    fprintf(stderr, "Failed to split: %s\n", infile);
    return 4;
  }

  for (size_t i = 0; i < frames.size(); ++i) {
    auto const filename = std::to_string(i) + ".celframe";
    auto const filepath = std::filesystem::path(outdir) / filename;
    auto const &frame = frames.at(i);

    if (!diabutil::write_file(diabutil::make_span(frame), filepath.c_str())) {
      fprintf(stderr, "Failed to save frame: %zu\n", i);
      return 5;
    }
  }

  return 0;
}
