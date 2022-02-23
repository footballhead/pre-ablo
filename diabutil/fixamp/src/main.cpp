#include <cstdio>
#include <diabutil/file.hpp>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std::string_literals;

#define MAPFLAG_TYPE 0xF

#define MAPFLAG_DIRT 0x40 << 8

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s file.amp\n", argv[0]);
    fprintf(stderr, "Mutate an AMP file to work in PR Demo\n");
    return 1;
  }

  auto const amp_file = argv[1];
  auto amp_file_contents = diabutil::read_file(amp_file);
  if (!amp_file_contents) {
    fprintf(stderr, "Failed to load: %s\n", amp_file);
    return 2;
  }

  // Each entry is a WORD (16 bits)
  for (size_t byte_index = 0; byte_index < amp_file_contents->size();
       byte_index += 2) {
    auto word_index = byte_index / 2;
    // TODO: This will only work on little endian systems
    auto &automap_type = reinterpret_cast<unsigned short *>(
        amp_file_contents->data())[word_index];

    auto const type = automap_type & MAPFLAG_TYPE;
    auto const flags = automap_type ^ type;

    // PR demo only understands types 1-6, but retail defines types 8-12
    // specifically for caves e.g. 8 is "SW corner" so devilution uses two
    // flags: do_vert = TRUE and do_cave_horz = TRUE My guess is it has
    // something to do with caves having more TILs representing corners of
    // different orientations.
    switch (type) {
      case 8:   // corner
      case 9:   // corner
      case 10:  // left-facing fence, etc
      case 11:  // right-facing fence, etc
      case 12:  // corner
        automap_type = flags | MAPFLAG_DIRT;
        break;
    }
  }

  if (!diabutil::dump_to_disk(diabutil::make_span(*amp_file_contents),
                              amp_file)) {
    fprintf(stderr, "Failed to save, file could be corrupt: %s\n", amp_file);
    return 3;
  }

  return 0;
}