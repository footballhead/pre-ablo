#include <cstdio>
#include <diabutil/file.hpp>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std::string_literals;

//
// mini_tile and standard BS
//

struct mini_tile {
  uint16_t data;

  int frame_number() const { return data & 0x0FFF; }
  int frame_type() const { return (data & 0x7000) >> 12; }
};

bool operator==(mini_tile const& a, mini_tile const& b) {
  return a.data == b.data;
}

namespace std {
template <>
struct hash<mini_tile> {
  std::size_t operator()(mini_tile const& mt) const noexcept { return mt.data; }
};
}  // namespace std

//
// entry point
//

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s file.min\n", argv[0]);
    fprintf(
        stderr,
        "Mutate a celpart so that it can be displayed properly by prdemo\n");
    return 1;
  }

  //
  // Read and parse min file
  //

  auto min_file_contents = read_entire_file(argv[1]);

  std::unordered_set<mini_tile> mini_tiles;
  for (int i = 0; i < min_file_contents.size(); i += 2) {
    uint16_t buffer =
        *reinterpret_cast<uint16_t const*>(min_file_contents.data() + i);
    auto mt = mini_tile{buffer};
    mini_tiles.insert(mt);
  }

  //
  // Use min file info to surgically apply fixes
  //

  for (auto const& min : mini_tiles) {
    if (min.frame_type() == 2) {  // left triangle
      auto const filename = std::to_string(min.frame_number()) + ".celpart";
      fprintf(stderr, "Fixing %s\n", filename.c_str());
      auto cel = read_entire_file(filename.c_str());

      cel.erase(begin(cel), std::next(begin(cel), 2));
      cel.erase(std::next(begin(cel), 0x6), std::next(begin(cel), 0x8));
      cel.erase(std::next(begin(cel), 0x14), std::next(begin(cel), 0x16));
      cel.erase(std::next(begin(cel), 0x2a), std::next(begin(cel), 0x2c));
      cel.erase(std::next(begin(cel), 0x48), std::next(begin(cel), 0x4a));
      cel.erase(std::next(begin(cel), 0x6e), std::next(begin(cel), 0x70));
      cel.erase(std::next(begin(cel), 0x9c), std::next(begin(cel), 0x9e));
      cel.erase(std::next(begin(cel), 0xd2), std::next(begin(cel), 0xd4));
      cel.erase(std::next(begin(cel), 0x110), std::next(begin(cel), 0x112));
      cel.erase(std::next(begin(cel), 0x14a), std::next(begin(cel), 0x14c));
      cel.erase(std::next(begin(cel), 0x17c), std::next(begin(cel), 0x17e));
      cel.erase(std::next(begin(cel), 0x1a6), std::next(begin(cel), 0x1a8));
      cel.erase(std::next(begin(cel), 0x1c8), std::next(begin(cel), 0x1ca));
      cel.erase(std::next(begin(cel), 0x1e2), std::next(begin(cel), 0x1e4));
      cel.erase(std::next(begin(cel), 0x1f4), std::next(begin(cel), 0x1f6));
      cel.erase(std::next(begin(cel), 0x1fe), std::next(begin(cel), 0x200));

      std::ofstream out{filename, std::ios_base::binary};
      out.write(reinterpret_cast<char const*>(cel.data()), cel.size());
    } else if (min.frame_type() == 3) {  // right triangle
      auto const filename = std::to_string(min.frame_number()) + ".celpart";
      fprintf(stderr, "Fixing %s\n", filename.c_str());
      auto cel = read_entire_file(filename.c_str());

      cel.erase(std::next(begin(cel), 0x2), std::next(begin(cel), 0x4));
      cel.erase(std::next(begin(cel), 0xc), std::next(begin(cel), 0xe));
      cel.erase(std::next(begin(cel), 0x1e), std::next(begin(cel), 0x20));
      cel.erase(std::next(begin(cel), 0x38), std::next(begin(cel), 0x3a));
      cel.erase(std::next(begin(cel), 0x5a), std::next(begin(cel), 0x5c));
      cel.erase(std::next(begin(cel), 0x84), std::next(begin(cel), 0x86));
      cel.erase(std::next(begin(cel), 0xb6), std::next(begin(cel), 0xb8));
      cel.erase(std::next(begin(cel), 0xf0), std::next(begin(cel), 0xf2));
      cel.erase(std::next(begin(cel), 0x12e), std::next(begin(cel), 0x130));
      cel.erase(std::next(begin(cel), 0x164), std::next(begin(cel), 0x166));
      cel.erase(std::next(begin(cel), 0x192), std::next(begin(cel), 0x194));
      cel.erase(std::next(begin(cel), 0x1b8), std::next(begin(cel), 0x1ba));
      cel.erase(std::next(begin(cel), 0x1d6), std::next(begin(cel), 0x1d8));
      cel.erase(std::next(begin(cel), 0x1ec), std::next(begin(cel), 0x1ee));
      cel.erase(std::next(begin(cel), 0x1fa), std::next(begin(cel), 0x1fc));
      cel.erase(std::next(begin(cel), 0x200), end(cel));

      std::ofstream out{filename, std::ios_base::binary};
      out.write(reinterpret_cast<char const*>(cel.data()), cel.size());
    } else if (min.frame_type() == 4) {  // left triangle to wall
      auto const filename = std::to_string(min.frame_number()) + ".celpart";
      fprintf(stderr, "Fixing %s\n", filename.c_str());
      auto cel = read_entire_file(filename.c_str());

      cel.erase(begin(cel), std::next(begin(cel), 2));
      cel.erase(std::next(begin(cel), 0x6), std::next(begin(cel), 0x8));
      cel.erase(std::next(begin(cel), 0x14), std::next(begin(cel), 0x16));
      cel.erase(std::next(begin(cel), 0x2a), std::next(begin(cel), 0x2c));
      cel.erase(std::next(begin(cel), 0x48), std::next(begin(cel), 0x4a));
      cel.erase(std::next(begin(cel), 0x6e), std::next(begin(cel), 0x70));
      cel.erase(std::next(begin(cel), 0x9c), std::next(begin(cel), 0x9e));
      cel.erase(std::next(begin(cel), 0xd2), std::next(begin(cel), 0xd4));

      std::ofstream out{filename, std::ios_base::binary};
      out.write(reinterpret_cast<char const*>(cel.data()), cel.size());
    } else if (min.frame_type() == 5) {  // right triangle to wall
      auto const filename = std::to_string(min.frame_number()) + ".celpart";
      fprintf(stderr, "Fixing %s\n", filename.c_str());
      auto cel = read_entire_file(filename.c_str());

      cel.erase(std::next(begin(cel), 0x2), std::next(begin(cel), 0x4));
      cel.erase(std::next(begin(cel), 0xc), std::next(begin(cel), 0xe));
      cel.erase(std::next(begin(cel), 0x1e), std::next(begin(cel), 0x20));
      cel.erase(std::next(begin(cel), 0x38), std::next(begin(cel), 0x3a));
      cel.erase(std::next(begin(cel), 0x5a), std::next(begin(cel), 0x5c));
      cel.erase(std::next(begin(cel), 0x84), std::next(begin(cel), 0x86));
      cel.erase(std::next(begin(cel), 0xb6), std::next(begin(cel), 0xb8));
      cel.erase(std::next(begin(cel), 0xf0), std::next(begin(cel), 0xf2));

      std::ofstream out{filename, std::ios_base::binary};
      out.write(reinterpret_cast<char const*>(cel.data()), cel.size());
    }
  }

  return 0;
}