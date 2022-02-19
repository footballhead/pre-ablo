#pragma once

#include <cinttypes>
#include <string>
#include <vector>

std::vector<uint8_t> read_entire_file(std::string const &filename);

void dump_to_disk(std::vector<uint8_t> const &data,
                  std::string const &filename);
