#include "patches.hpp"

#include "enums.hpp"
#include "functions.hpp"
#include "macros.hpp"
#include "util.hpp"
#include "variables.hpp"

#include <cstdlib>

#define PATCH_NAME z_hook
DESCRIBE_PATCH("Do something when Z is pressed")

namespace {

void __fastcall z_press_hook()
{
    // Reroll current level
    plr_pLvlVisited[*currlevel] = FALSE;
    plr_pSeedTbl[*currlevel] = rand();
    NetSendCmd(*myplr, CMD_WARP, FALSE, *leveltype, *currlevel);
}

} // namespace

PATCH_MAIN
{
    bool ok = true;

    ok &= nop(0x0048722B, 0x00487251);

    ok &= patch_call(0x0048722B, (void*)z_press_hook);

    for (auto i = 0; AllItemsList[i].iLoc != -1; ++i) {
        // Don't drop
        AllItemsList[i].iRnd = FALSE;
        // ... unless you're heavy armor/elixir of strength, in which case always drop
        if (AllItemsList[i].itype == ITYPE_HARMOR || AllItemsList[i].iMiscId == 8) {
            AllItemsList[i].iRnd = TRUE;
            AllItemsList[i].iMinMLvl = 1;
        }
    }

    return ok;
}

