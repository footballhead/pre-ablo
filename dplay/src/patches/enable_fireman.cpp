#include "patches.hpp"

#include "enums.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME enable_fireman
DESCRIBE_PATCH("The cut Incinerator enemy will appear on dlvl 8-10.\n\nToggling will ruin old saves, it's recommended to start a new character.")

PATCH_MAIN
{
    MonstAvailTbl[MT_INCIN] = MAT_YES;
    return true;
}
