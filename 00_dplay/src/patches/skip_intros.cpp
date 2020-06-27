#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME skip_intros
DESCRIBE_PATCH("Skip the logos, quotes, and intro animation when launching the game.")

PATCH_MAIN
{
    // This is the data segment, no need to adjust permissions
    *(uint32_t*)0x004BC160 = FALSE;
    return true;
}
