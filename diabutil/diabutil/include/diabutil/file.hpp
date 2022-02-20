#pragma once

#include <cinttypes>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

/// @deprecated
std::vector<uint8_t> read_entire_file(std::string const &filename);

void dump_to_disk(std::vector<uint8_t> const &data,
                  std::string const &filename);

namespace diabutil {

/// Read an entire binary file into memory.
///
/// @param filename Filepath to load
/// @returns On success, a valid optional of all the bytes in the file. On
/// error, std::nullopt
std::optional<std::vector<std::byte>> read_file(std::string_view filename);

}  // namespace diabutil