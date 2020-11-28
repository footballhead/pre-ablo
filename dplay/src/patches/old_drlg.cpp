#include "patches.hpp"

#include "functions.hpp"
#include "macros.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME old_drlg
DESCRIBE_PATCH(R"txt(Enable the old Alpha DRLG for cathedral levels 1-4 in new games.

Known issues:

- The entrance to the Skeleton King's Lair does not generate on dlvl 3. This probably wasn't implemented at this point in development so this will not be fixed.

- Since the Skeleton King cannot be completed, the Magic Rock will not generate.)txt")

void __fastcall InitObjects_patch()
{
    // Displaced
    AddL1Objs(0, 0, MAXDUNX, MAXDUNY);

    // Here's the fix
    if (quests[QTYPE_MAZE]._qlevel == *currlevel) {
        AddMazeBook(0, 0, MAXDUNX, MAXDUNY, *setpc_x, *setpc_y, *setpc_w + *setpc_x + 1, *setpc_h + *setpc_y + 1, 11);
    }
}

PATCH_MAIN
{
    bool ok = true;

    // Make InitLevelType use old DRLG for levels 1-4
    constexpr uint8_t mov_eax_1[] = {0xB8, 0x01, 0x00, 0x00, 0x00};
    ok &= patch_bytes(0x00488473, mov_eax_1, sizeof(mov_eax_1));

    // 3 is skulpile, it must always load to avoid crashing
    AllObjects[3].oload = 1; // OBJ_MUST
    AllObjects[3].otheme = -1;

    // When using devmode, spawn in old DRLG
    constexpr uint8_t mov_leveltype_1[] = {0xC7, 0x05, 0x48, 0x36, 0x4E, 0x00, 0x01, 0x00, 0x00, 0x00};
    ok &= patch_bytes(0x004894AF, mov_leveltype_1, sizeof(mov_leveltype_1));

    // fix Tome for catacombs not spawning on level 4
    ok &= nop(0x0045957E, 0x0045958B); // Dsiplace AddL1Objs(0, 0, MAXDUNX, MAXDUNY);
    ok &= patch_call(0x0045957E, InitObjects_patch);

    // TODO mark the skeleton king quest as completed? at least so you can do magic rock

    return ok;
}
