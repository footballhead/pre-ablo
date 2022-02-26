#pragma once

#include <cinttypes>
#include <cstddef>
#include <diabutil/types.hpp>
#include <optional>
#include <string>
#include <vector>

namespace diabutil {

/// Read an entire binary file into memory.
///
/// @param filename Filepath to load
/// @returns On success, a valid array of all the bytes in the file. On
/// error, std::nullopt
std::optional<byte_vector> read_file(char const *filename);

/// Persist a byte buffer to a file on disk
///
/// @param data The bytes to persist
/// @param filename Filepath to save bytes in
/// @returns true on success, false on error
bool write_file(byte_span data, char const *filename);

}  // namespace diabutil