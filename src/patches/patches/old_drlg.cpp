#include "functions.hpp"
#include "macros.hpp"
#include "patches.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME old_drlg
DESCRIBE_PATCH(
    R"txt(Enables the old Alpha DRLG for cathedral levels 1-4 in new games.

Known issues:

- The entrance to the Skeleton King's Lair does not generate on dlvl 3. This probably wasn't implemented at this point in development so this will not be fixed.)txt")

void __fastcall InitObjects_patch() {
  // Displaced
  AddL1Objs(0, 0, MAXDUNX, MAXDUNY);

  // Here's the fix
  if (quests[QTYPE_MAZE]._qlevel == currlevel) {
    AddMazeBook(0, 0, MAXDUNX, MAXDUNY, *setpc_x, *setpc_y,
                *setpc_w + *setpc_x + 1, *setpc_h + *setpc_y + 1, 11);
  }
}

PATCH_MAIN {
  // 3 is skulpile, it must always load to avoid crashing
  AllObjects[3].oload = 1;  // OBJ_MUST
  AllObjects[3].otheme = -1;

  // InitLevelType: Use old DRLG (leveltype==1) for levels 1-4
  bool ok = patch_dword(0x00488473 + 1, 1); // +1 for opcode

  // InitLevels: When using devmode, spawn in old DRLG.
  ok &= patch_dword(0x004894AF + 6, 1); // +2 for mov opcode, +4 for address

  // InitObjects: Spawn Tome on level 4 so that the player can open the stairs.
  ok &= nop(0x0045957E, 0x0045958B);
  ok &= patch_call(0x0045957E, InitObjects_patch);

  // TalkToTowners: Remove beating The Skeleton King as a requirement for
  // starting The Magic Rock. Since the player can never find The Skeleton King,
  // this is necessary for them to do the quest.
  ok &= nop(0x0044A9F5, 0x0044AA05);

  return ok;
}
