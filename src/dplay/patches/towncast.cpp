#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME towncast
DESCRIBE_PATCH("Allows casting in town")

PATCH_MAIN
{
    bool ok = true;

    ok &= nop(0x00470CD1, 0x00470CE3);

    return ok;
}
