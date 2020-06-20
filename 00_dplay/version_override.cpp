#include "patches.hpp"

#include "version.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace {
auto version = reinterpret_cast<char*>(0x004AA6F4);
} // namespace

void version_override_main()
{
    strncpy(version, PRDE_VERSION, 17);
}
