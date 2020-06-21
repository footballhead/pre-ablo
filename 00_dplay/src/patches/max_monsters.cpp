#include "patches.hpp"

#include "util.hpp"

void max_monsters_main()
{
    // nop monstimgtot checks
    // This means levels can have up to 16 monsters!
    nop(0x00401366, 0x00401373); // cmp and jge
    nop(0x004013A4, 0x004013AF); // cmp and jg
}

// others:
// max magic find
// boost unique monsters
// randomize level types (would this mess with quests?)
// randomize uniques
