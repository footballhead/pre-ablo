#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

void old_drlg_main()
{
    bool ok = true;

    // Make InitLevelType use old DRLG for levels 1-4
    constexpr uint8_t mov_eax_1[] = {0xB8, 0x01, 0x00, 0x00, 0x00};
    ok &= patch_bytes(0x00488473, mov_eax_1, sizeof(mov_eax_1));

    // 3 is skulpile, it must always load to avoid crash
    AllObjects[3].oload = 1; // OBJ_MUST
    AllObjects[3].otheme = -1;

    // When using devmode, spawn in old DRLG
    constexpr uint8_t mov_leveltype_1[] = {0xC7, 0x05, 0x48, 0x36, 0x4E, 0x00, 0x01, 0x00, 0x00, 0x00};
    ok &= patch_bytes(0x004894AF, mov_leveltype_1, sizeof(mov_leveltype_1));

    printf("%s %s\n", __func__, ok ? "success" : "fail");
}
