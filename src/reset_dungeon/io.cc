#include "io.h"

#include <windows.h>

#include <cstddef>
#include <fstream>
#include <ios>
#include <optional>
#include <string_view>
#include <vector>

std::optional<std::vector<std::byte>> ReadFromFile(std::string_view filename) {
  std::ifstream in{std::string(filename), std::ios_base::binary};
  if (!in.good()) {
    MessageBox(NULL, TEXT("Could not open the save file for reading."),
               TEXT("Error"), MB_OK | MB_ICONERROR);
    return std::nullopt;
  }

  in.seekg(/*off=*/0, std::ios::end);
  const int length = static_cast<int>(in.tellg());
  in.seekg(/*off=*/0);

  std::vector<std::byte> contents(length, std::byte(0));
  in.read(reinterpret_cast<char*>(contents.data()), contents.size());
  if (!in.good()) {
    MessageBox(NULL, TEXT("Could not read the save file."), TEXT("Error"),
               MB_OK | MB_ICONERROR);
    return std::nullopt;
  }

  return contents;
}

bool WriteToFile(std::string_view filename,
                 const std::vector<std::byte>& buffer) {
  std::ofstream out{std::string(filename), std::ios_base::binary};
  if (!out.good()) {
    MessageBox(NULL, TEXT("Could not open the save file for writing."),
               TEXT("Error"), MB_OK | MB_ICONERROR);
    return false;
  }

  out.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
  if (!out.good()) {
    MessageBox(NULL, TEXT("Could not write to the save file."), TEXT("Error"),
               MB_OK | MB_ICONERROR);
    return false;
  }

  out.close();
  if (!out.good()) {
    MessageBox(NULL, TEXT("Could not close the save file."), TEXT("Error"),
               MB_OK | MB_ICONERROR);
    return false;
  }

  return true;
}
