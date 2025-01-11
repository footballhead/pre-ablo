#include "functions.hpp"
#include "patches.hpp"
#include "structs.hpp"
#include "util.hpp"
#include "variables.hpp"

// TODO combine all the stone curse fixes into one patch

#define PATCH_NAME stone_curse_target_fix
DESCRIBE_PATCH("Fixes Stone Curse not working on some monsters.");

#define MM_STONE 15
#define SPL_STONE 8

namespace {

// It was easier for me to copy the decomp here and make the necessary code
// change than to write a targeted fix in machine code.
void __fastcall AddStone_Fixed(int mi, int sx, int sy, int dx, int dy,
                               int midir, int micaster, int id, int midam) {
  // offsets into crawl table for rings 0 (same tile) through 5 (up to 5 tiles
  // away)
  int CrawlNum[6] = {0, 3, 12, 45, 94, 159};
  int j, l, i, mid, k;

  missile[mi]._misource = id;
  missile[mi]._miAnimFlags = 0;
  missile[mi]._miAnimData = NULL;

  // Start seraching from a monster up to 5 rings away
  for (i = 0; i < 6; i++) {
    k = CrawlNum[i];  // k is offset to beginning of ring in CrawlTable
    l = k + 1;        // l (or k+1) is the offset to the first (x,y) pair
                      // l will be the offset for X
                      // l+1 is the offset for Y
    // The first entry CrawlTable[CrawlNum[foo]] is the # of (x,y) pairs in the
    // ring
    for (j = (BYTE)CrawlTable[k]; j > 0; j--) {
      mid = dMonster[dx + CrawlTable[l]][dy + CrawlTable[l + 1]];
      // Check for invalid monster ID before continuing. This is the actual
      // bugfix and the reason I decompiled this entire function.
      if (mid != 0) {
        // Only after we know that the monster ID is valid do we translate from
        // dMonster index into monster index.
        if (mid > 0) {
          mid--;
        } else {
          mid = -(mid + 1);
        }

        // Stop looking, we found one!
        // Combined with the break, this will escape both for-loops
        j = -99;
        i = 6;

        // missile var1 is backup monster mmode
        missile[mi]._miVar1 = monster[mid]._mmode;
        // missile var2 is the monster ID we've turned to stone
        missile[mi]._miVar2 = mid;

        monster[mid]._mmode = MM_STONE;
        break;
      }

      // Keep looking, advance l to next (x,y) pair
      l += 2;
    }
  }

  if (j != -99) {
    // No monster in 5-ring radius, delete the missile
    missile[mi]._miDelFlag = TRUE;
    DeleteMissile(mi, mi);
  } else {
    // Found a monster, keep the missile to track the effect
    missile[mi]._mix = CrawlTable[l] + dx;
    missile[mi]._miy = CrawlTable[l + 1] + dy;
    missile[mi]._misx = missile[mi]._mix;
    missile[mi]._misy = missile[mi]._miy;

    missile[mi]._mirange = plr[id]._pLevel << 4;
    for (j = GetSpellLevel(id, SPL_STONE); j > 0; j--) {
      missile[mi]._mirange += ((missile[mi]._mirange << 4) >> 3);
    }
  }
}

}  // namespace

PATCH_MAIN {
  // AddMissile: Replace the original AddStone with our version which has the
  // fix
  return patch_call(0x0043B29B, AddStone_Fixed);
}
