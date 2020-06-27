#include "patches.hpp"

#include "util.hpp"
#include "version.h"

#include <cstring>

#define PATCH_NAME version_override
DESCRIBE_PATCH("Replaces the version number on the main menu and in-game (press V) with the PR Demo Enhanced release number. Purely cosmetic and vain.")

namespace {
auto version = reinterpret_cast<char*>(0x004AA6F4);
constexpr auto version_size = 17; // strlen("Pre-Release Demo") + 1
} // namespace

PATCH_MAIN
{
    bool ok = true;

    // TODO: If we wanted to be extra safe, we could use our own local variable and get to set the size
    strncpy(version, PRDE_VERSION, version_size);

    // Always show this on main menu (regardless of fullgame/demo mode)
    ok &= patch_push(0x0041920F, version);
    // Always show this in-game when pressing V (again, regardless of demo mode)
    ok &= patch_push(0x004873CB, version);

    return ok;
}
