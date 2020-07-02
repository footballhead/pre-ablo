#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME max_monster_types
DESCRIBE_PATCH("Allow up to 16 monster types per level. Has the side effect of generating more uniques per level.")

PATCH_MAIN
{
    bool ok = true;
    // nop monstimgtot checks
    // This means levels can have up to 16 monsters!
    ok &= nop(0x00401366, 0x00401373); // cmp and jge
    ok &= nop(0x004013A4, 0x004013AF); // cmp and jg
    return ok;
}
