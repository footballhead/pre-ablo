#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME no_tp_light
DESCRIBE_PATCH("Disables the light emitted by the town portal. This works around a crash on load if the player was in the dungeon and had cast town portal.")

PATCH_MAIN
{
    bool ok = true;
    ok &= nop(0x0043DE87, 0x0043DF14);
    return ok;
}
