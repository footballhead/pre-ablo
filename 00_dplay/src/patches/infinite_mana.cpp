#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME infinite_mana
DESCRIBE_PATCH("Spells don't cost mana")

PATCH_MAIN
{
    bool ok = true;
    ok &= nop(0x0045405A, 0x00454067);
    return ok;
}
