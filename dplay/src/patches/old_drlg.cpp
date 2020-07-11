#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME old_drlg
DESCRIBE_PATCH(R"txt(Enable the old Alpha DRLG for the cathedral levels 1-4.

Known issues:

- The Tome does not generate on dlvl 4, which means you cannot progress to the catacombs. This will be fixed at some point.

- The entrance to the Skeleton King's Lair does not generate on dlvl 3. This probably wasn't implemented at this point in development so this will not be fixed.)txt")

PATCH_MAIN
{
    bool ok = true;

    // Make InitLevelType use old DRLG for levels 1-4
    constexpr uint8_t mov_eax_1[] = {0xB8, 0x01, 0x00, 0x00, 0x00};
    ok &= patch_bytes(0x00488473, mov_eax_1, sizeof(mov_eax_1));

    // 3 is skulpile, it must always load to avoid crash
    AllObjects[3].oload = 1; // OBJ_MUST
    AllObjects[3].otheme = -1;

    // When using devmode, spawn in old DRLG
    constexpr uint8_t mov_leveltype_1[] = {0xC7, 0x05, 0x48, 0x36, 0x4E, 0x00, 0x01, 0x00, 0x00, 0x00};
    ok &= patch_bytes(0x004894AF, mov_leveltype_1, sizeof(mov_leveltype_1));

    // TODO fix Tome for catacombs on level 4
    // TODO mark the skeleton king quest as completed? at least so you can do magic rock and bone chamber in catacombs

    return ok;
}
