#include "patches.hpp"

#include "enums.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME enable_unfinished_monsters
DESCRIBE_PATCH(R"txt(Enables cut monsters which look like they're "unfinished". They all appear on dlvl 13-15 with 10-20 health:

- Mage

- Sneaky Lord

- Goat Lord

- Diablo

Toggling will ruin old saves, so it's recommended to start a new character.)txt")

PATCH_MAIN
{
    // TODO: Disclaimer about broken walk cycle (using mages.cel for walk because its 20 frames)
    MonstAvailTbl[MT_COUNSLR] = MAT_YES;
    MonstAvailTbl[MT_LRDSAYTR] = MAT_YES;
    MonstAvailTbl[MT_INVILORD] = MAT_YES;
    // TODO: Adjust get hit and walk frames from 16 to 6
    MonstAvailTbl[MT_INVILORD] = MAT_YES;
    return true;
}
