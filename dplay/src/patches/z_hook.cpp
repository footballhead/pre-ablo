#include "patches.hpp"

#include "functions.hpp"
#include "macros.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME z_hook
DESCRIBE_PATCH("Do something when Z is pressed")

namespace {

void __fastcall z_press_hook()
{
    // Give me a quick yes/no on whether the book was generated
    // (so I don't have to go hunting for it)
    // with the understanding that this will only work when the player is on level 4
    ObjectStruct* book_obj = nullptr;
    for (auto i = 0; i < MAXOBJECTS; ++i) {
        if (object[i]._otype == OBJ_BOOKMAZE) {
            book_obj = &object[i];
            break;
        }
    }

    if (book_obj) {
        printf("Found book at (%d, %d)\n", book_obj->_ox, book_obj->_oy);
    } else {
        printf("No book obj :(\n");
    }

    // Go to level 4 (dungeon type 1/old cathedral)
    NetSendCmd(*myplr, CMD_NEWLVL, 0, 1, 4);
    // Regenerate the level
    plr_pLvlVisited[4] = FALSE;
    // Regenerate with a new seed
    plr_pSeedTbl[4] = rand();
}

} // namespace

PATCH_MAIN
{
    bool ok = true;

    ok &= nop(0x0048722B, 0x00487251);

    ok &= patch_call(0x0048722B, (void*)z_press_hook);

    return ok;
}

