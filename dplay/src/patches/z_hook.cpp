#include "patches.hpp"

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

    return ok;
}

