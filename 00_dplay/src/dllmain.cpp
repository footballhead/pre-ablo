#include "patches.hpp"
#include "version.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdio>

extern "C" BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
#ifndef NDEBUG
        AllocConsole();
        freopen("CONOUT$", "wb", stdout);
#endif
        // TODO: redirect to file if ndebug?

        always_load_flare_main();
        automap_fix_main();
#ifndef NDEBUG
        // cheat_main(); // should be off
        devmode_main(); // should be off
#endif
        fullgame_main();
        infraring_fix_main();
        // max_monsters_main(); // should be off
        mega_fix_main();
        // music_nompq_fix_main();  // should be off
        // savegame_patch_fix_main(); // TODO apparently this doesn't work with town portals (esp setlevels)
        skip_intros_main();
        snake_frame_fix_main();
        stone_curse_missile_fix_main();
        thunder_demon_missile_fix_main();
        too_much_hp_crash_main();
        tp_setlevel_fix_main();
        undead_crown_main();
        version_override_main();
        // window_main(); // should be off
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
