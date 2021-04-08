#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME plrgfx_frame_fix
DESCRIBE_PATCH("Adjust warrior \"unarmed with shield\" death animation from 20 to 19 frames to match the broken wludt.cel we have.")

PATCH_MAIN
{
    bool ok = true;

    // Old:
    //   C7 80 D8 5C 60 00 14 00 00 00                 mov     plr_pDFrames[eax], 20
    // New:
    //   C7 80 D8 5C 60 00 13 00 00 00                 mov     plr_pDFrames[eax], 19
    uint8_t new_death_frames = 13;
    ok &= patch_bytes(0x0046691D + 6, &new_death_frames, 1);

    return ok;
}
