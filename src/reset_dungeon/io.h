#pragma once

#include <cstddef>
#include <optional>
#include <string_view>
#include <vector>


// Returns the entire contents of `filename` on succes, `std::nullopt` on error.
//
// On error, the program pauses and a message box is show to the user.
std::optional<std::vector<std::byte>> ReadFromFile(std::string_view filename);

// Writes `buffer` to `filename. Returns `true` on success, `false` on error.
//
// On error, the program pauses and a message box is show to the user.
bool WriteToFile(std::string_view filename,
                 const std::vector<std::byte>& buffer);
