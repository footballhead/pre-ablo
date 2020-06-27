#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME max_monsters
DESCRIBE_PATCH("Removes the graphics size limitation on monster types, allowing up to 16 monsters per level.")

PATCH_MAIN
{
    bool ok = true;
    // nop monstimgtot checks
    // This means levels can have up to 16 monsters!
    ok &= nop(0x00401366, 0x00401373); // cmp and jge
    ok &= nop(0x004013A4, 0x004013AF); // cmp and jg
    return ok;
}

// others:
// max magic find
// boost unique monsters
// randomize level types (would this mess with quests?)
// randomize uniques
