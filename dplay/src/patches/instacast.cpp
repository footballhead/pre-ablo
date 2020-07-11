#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME instacast
DESCRIBE_PATCH("Cast spells instantly")

PATCH_MAIN
{
    bool ok = true;

    // Remove delay on cast
    ok &= nop(0x0046DC66, 0x0046DC72);
    // Immediately end cast animation
    ok &= nop(0x0046DE93, 0x0046DE9F);

    return ok;
}
