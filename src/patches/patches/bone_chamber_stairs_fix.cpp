#include <windows.h>

#include "patches.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME bone_chamber_stairs_fix
DESCRIBE_PATCH("Makes the Bone Chamber stairs easier to use.")

#define WM_DIABNEXTLVL WM_USER + 2
#define WM_DIABPREVLVL WM_USER + 3

namespace {

// The upshot of this fix is that the "Up to level 6" text will be gone for the
// bone chamber stairs on level 7. The player will need to know to click the
// base of the stairs in order to activate the trigger.
BOOL ForceL2Trig_Fixed() {
  int i, j;

  for (i = 0; L2UpList[i] != -1; i++) {
    if (dPiece[cursmx][cursmy] == L2UpList[i]) {
      for (j = 0; j < numtrigs; j++) {
        // The stairs to the bone chamber uses stairs up tiles found in L2UpList
        // but doesn't have an associated trigger. The actual "trigger" location
        // is stored in the quest struct at (_qtx, _qty) and the level
        // transition is handled by CheckQuests.
        if (trigs[j]._tmsg == WM_DIABPREVLVL) {
          // This is the fix from retail: the location of the trigger must be
          // within a 4 tile tolerance for it to actually be the stairs up
          int dx = abs(trigs[j]._tx - cursmx);
          int dy = abs(trigs[j]._ty - cursmy);
          if (dx < 4 && dy < 4) {
            sprintf(infostr, "Up to level %i", currlevel - 1);
            cursmx = trigs[j]._tx;
            cursmy = trigs[j]._ty;
            return TRUE;
          }
        }
      }
    }
  }

  for (i = 0; L2DownList[i] != -1; i++) {
    if (dPiece[cursmx][cursmy] == L2DownList[i]) {
      sprintf(infostr, "Down to level %i", currlevel + 1);

      for (j = 0; j < numtrigs; j++) {
        if (trigs[j]._tmsg == WM_DIABNEXTLVL) {
          cursmx = trigs[j]._tx;
          cursmy = trigs[j]._ty;
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}

}  // namespace

PATCH_MAIN {
  // CheckTrigForce: Use our fixed catacombs trigger logic. While the same
  // function is also used in hell, nothing in hell requires the fix.

  return patch_call(0x004731A3, ForceL2Trig_Fixed);
}
