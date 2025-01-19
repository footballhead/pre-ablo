#include "patches.hpp"

#include "enums.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME enable_cut_monsters
DESCRIBE_PATCH(R"txt(Enables cut monsters which appear to be "finished" (they have proper stats). This includes:

- Incinerator (the basis for Elemental?), which will appear on dlvl 8-10.

- Unraveler (found in Hellfire), which will appear on dlvl 12-14.

Toggling will ruin old saves, so it's recommended to start a new character.)txt")

PATCH_MAIN
{
    MonstAvailTbl[MT_INCIN] = MAT_YES;
    MonstAvailTbl[MT_UNRAV] = MAT_YES;
    return true;
}
