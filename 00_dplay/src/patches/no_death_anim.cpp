#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME no_death_anim
DESCRIBE_PATCH("Prevent crash on Magma Demon death by disabling all death animations")

PATCH_MAIN
{
    bool ok = true;
    // Don't play death animation, jump straight to corpse
    // Might need to add a conditional to be selective about enemies...
    ok &= nop(0x00406EFE, 0x00406F1F);
    return ok;
}
