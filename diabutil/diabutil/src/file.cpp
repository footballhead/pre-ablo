#include <diabutil/file.hpp>
#include <fstream>
#include <stdexcept>

std::vector<uint8_t> read_entire_file(std::string const &filename) {
  std::ifstream in{filename, std::ios_base::binary};
  if (!in.good()) {
    throw std::runtime_error{"Failed to open file: " + filename};
  }
  return std::vector<uint8_t>(std::istreambuf_iterator<char>(in),
                              std::istreambuf_iterator<char>());
}

void dump_to_disk(std::vector<uint8_t> const &data,
                  std::string const &filename) {
  std::ofstream out{filename, std::ios_base::binary};
  if (!out.good()) {
    throw std::runtime_error{"Failed to open file: " + filename};
  }
  out.write(reinterpret_cast<char const *>(data.data()), data.size());
  if (!out.good()) {
    throw std::runtime_error{"Failed to write data"};
  }
}

namespace diabutil {

std::optional<std::vector<std::byte>> read_file(char const *filename) {
  // Open file (`ate` puts internal marker at EOF so we can tellg for size)
  std::ifstream in{filename, std::ios_base::binary | std::ios_base::ate};
  if (!in.good()) {
    return std::nullopt;
  }

  // Get size. (StackOverflow will try to convince you that this is not the way
  // to go but it's literally in cplusplus.com:
  // https://www.cplusplus.com/reference/istream/istream/tellg/)
  auto const file_size = in.tellg();
  in.seekg(0, std::ios::beg);

  // Read file contents
  auto buffer = std::vector<std::byte>(file_size, std::byte{0});
  if (!in.read(reinterpret_cast<char *>(buffer.data()), file_size)) {
    return std::nullopt;
  }

  return buffer;
}

bool dump_to_disk(span<std::byte> data, char const *filename) {
  std::ofstream out{filename, std::ios_base::binary};
  if (!out.good()) {
    return false;
  }

  out.write(reinterpret_cast<char const *>(data.data), data.size);
  if (!out.good()) {
    return false;
  }

  return true;
}

}  // namespace diabutil