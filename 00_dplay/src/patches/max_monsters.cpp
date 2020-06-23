#include "patches.hpp"

#include "util.hpp"

void max_monsters_main()
{
    bool ok = true;
    // nop monstimgtot checks
    // This means levels can have up to 16 monsters!
    ok &= nop(0x00401366, 0x00401373); // cmp and jge
    ok &= nop(0x004013A4, 0x004013AF); // cmp and jg
    printf("%s %s\n", __func__, ok ? "success" : "fail");
}

// others:
// max magic find
// boost unique monsters
// randomize level types (would this mess with quests?)
// randomize uniques