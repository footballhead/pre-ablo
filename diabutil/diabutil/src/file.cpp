#include <diabutil/file.hpp>
#include <fstream>
#include <stdexcept>

namespace diabutil {
namespace {

template <typename InType>
std::optional<byte_vector> read_file_impl(InType filename) {
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
  auto buffer = byte_vector(file_size, std::byte{0});
  if (!in.read(reinterpret_cast<char *>(buffer.data()), file_size)) {
    return std::nullopt;
  }

  return buffer;
}

template <typename InType>
bool write_file_impl(byte_span data, InType filename) {
  std::ofstream out{filename, std::ios_base::binary};
  if (!out.good()) {
    return false;
  }

  out.write(reinterpret_cast<char const *>(data.data), data.size);
  if (!out.good()) {
    return false;
  }

  out.close();
  if (!out.good()) {
    return false;
  }

  return true;
}

}  // namespace

std::optional<byte_vector> read_file(char const *filename) {
  return read_file_impl<char const *>(filename);
}

std::optional<byte_vector> read_file(std::filesystem::path const &filename) {
  return read_file_impl<std::filesystem::path const &>(filename);
}

bool write_file(byte_span data, char const *filename) {
  return write_file_impl<char const *>(data, filename);
}

bool write_file(byte_span data, std::filesystem::path const &filename) {
  return write_file_impl<std::filesystem::path const &>(data, filename);
}

}  // namespace diabutil