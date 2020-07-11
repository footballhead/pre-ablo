#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME tp_setlevel_fix
DESCRIBE_PATCH("Fixes town portals in Skeleton King Lair (level 3) and the Bone Chamber (level 7). Allows returning from town without the game still thinking that you're in town (which typically ends in a crash or a hang).")

namespace {

int* const setlevel = reinterpret_cast<int* const>(0x005765F8);
int* const currlevel = reinterpret_cast<int* const>(0x004CB0DC);
int* const portal_currlevel_or_setlvlnum = reinterpret_cast<int* const>(0x004FBFB8);
int* const setlvlnum = reinterpret_cast<int* const>(0x004E95AC);

void GetPortalLevel_companion_fix()
{
    // Up to this point, only the following is set:
    // * setlevel
    // * gamma
    // * leveltype

    if (*setlevel) {
        *setlvlnum = *portal_currlevel_or_setlvlnum;
        switch (*portal_currlevel_or_setlvlnum) {
        case 1: // sking lair
            *currlevel = 3;
            break;
        case 2: // bone chamber
            *currlevel = 7;
            break;
        }
    } else {
        *currlevel = *portal_currlevel_or_setlvlnum;
    }
}

} // namespace

PATCH_MAIN
{
    // TODO also need to store loctaion for setlevels
    bool ok = true;

    ok &= nop(0x0040DFA1, 0x0040DFCC);
    ok &= patch_call(0x0040DFA1, (void*)GetPortalLevel_companion_fix);

    return ok;
}
