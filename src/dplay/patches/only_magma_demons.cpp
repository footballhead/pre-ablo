#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME only_magma_demons
DESCRIBE_PATCH("The only monster that will generate is the Magma Demon.")

PATCH_MAIN
{
    bool ok = true;

    // translation to currlevel:
    // 15 * monsterdata[i].mMinDLvl / 30 + 1;

    // Prevent all enemies
    for (auto i = 0; i < 93; ++i) { // 93 different monsters
        auto& dat = monsterdata[i];
        dat.mMinDLvl = 50; // Arbitrarily high
        dat.mMaxDLvl = 50; // Arbitrarily high
    }

    // Enable the ones we care about
    for (auto i = 54; i <= 57; ++i) { // 93 different monsters
        auto& dat = monsterdata[i];
        dat.mMinDLvl = 0;
        dat.mMaxDLvl = 50; // Arbitrarily high
    }

    return ok;
}
