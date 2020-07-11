#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

#include <cstdlib>

#define PATCH_NAME randomize_leveltype
DESCRIBE_PATCH("Randomize level types, so level 2 might be Hell, level 8 might be caves, etc.")

namespace {

auto ObjMasterLoadList = reinterpret_cast<char const* const* const>(0x004AC158);

// Return a random dungeon level type.
int __fastcall InitLevelType_hook(int l)
{
    // TODO: Do I need to srand?
    // Keep some level types so quests work
    // TODO disable quests altogether?
    switch (l) {
    case 0: // town
        return 0; // make sure it stays town
    case 1: // level containing butcher
        return rand() % 2 ? 5 : 1; // must be one of the cathedrals
    case 2: // level 2 doesn't contain a quest but make sure it's not a cathedral to shake things up
        return rand() % 3 + 2;
    case 3: // level containing skeleton king
        return 5; // new cathedral
    case 4: // level containing Tome
        return 5; // new cathedral
    case 7: // level containing bone chamber
        return 2; // catacombs;
    default:
        // Otherwise, randomize it!
        return rand() % 5 + 1;
    }
}

} // namespace

PATCH_MAIN
{
    bool ok = true;

    // Patch InitLevelType to return random levels
    ok &= patch_jmp(0x00488442, (void*)InitLevelType_hook);

    // From old DRLG patch
    // 3 is skulpile, it must always load to avoid crash
    AllObjects[3].oload = 1; // OBJ_MUST
    AllObjects[3].otheme = -1;

    for (auto i = 0; AllObjects[i].oload != -1; ++i) {
        auto& obj = AllObjects[i];
        // Allow all objects to spawn
        obj.ominlvl = 1;
        obj.omaxlvl = 16;
    }

    return ok;
}
