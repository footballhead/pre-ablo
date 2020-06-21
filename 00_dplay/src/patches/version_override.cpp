#include "patches.hpp"

#include "util.hpp"
#include "version.h"

#include <cstring>

namespace {
auto version = reinterpret_cast<char*>(0x004AA6F4);
constexpr auto version_size = 17; // strlen("Pre-Release Demo") + 1
} // namespace

void version_override_main()
{
    strncpy(version, PRDE_VERSION, version_size);

    // Always show this on main menu (regardless of fullgame/demo mode)
    patch_push(0x0041920F, version);

    // Always show this in-game when pressing V (again, regardless of demo mode)
    patch_push(0x004873CB, version);
}
