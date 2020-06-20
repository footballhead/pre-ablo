#include "patches.hpp"

#include "version.h"

#include <cstring>

namespace {
auto version = reinterpret_cast<char*>(0x004AA6F4);
constexpr auto version_size = 17; // strlen("Pre-Release Demo")
} // namespace

void version_override_main()
{
    // This is the data segment, no need to adjust permissions
    strncpy(version, PRDE_VERSION, version_size);
}
