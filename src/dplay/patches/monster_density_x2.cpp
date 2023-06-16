#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME monster_density_x2
DESCRIBE_PATCH("Double the density of monsters per level.")

PATCH_MAIN
{
    bool ok = true;

    // Double monster density
    ok &= patch<uint32_t>(0x004031DE + 1, 15);

    return ok;
}
