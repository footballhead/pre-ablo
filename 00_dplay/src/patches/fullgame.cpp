#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME fullgame
DESCRIBE_PATCH("Removes demo limitations. This allows you to play as Rogue or Sorceror, and to venture farther than level 1.")

PATCH_MAIN
{
    // This is the data segment, no need to adjust permissions
    *(uint32_t*)0x004BC15C = FALSE;
    return true;
}
