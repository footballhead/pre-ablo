#pragma once

#include <cinttypes>
#include <cstddef>
#include <diabutil/span.hpp>
#include <optional>
#include <string>
#include <vector>

/// @deprecated
std::vector<uint8_t> read_entire_file(std::string const &filename);

/// @deprecated
void dump_to_disk(std::vector<uint8_t> const &data,
                  std::string const &filename);

namespace diabutil {

/// Read an entire binary file into memory.
///
/// @param filename Filepath to load
/// @returns On success, a valid array of all the bytes in the file. On
/// error, std::nullopt
std::optional<std::vector<std::byte>> read_file(char const *filename);

/// Persist a buffer to a file on disk
///
/// @param data The bytes to persist
/// @param filename Filepath to save bytes in
/// @returns true on success, false on error
bool dump_to_disk(span<std::byte> data, char const *filename);

}  // namespace diabutil