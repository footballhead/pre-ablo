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

        always_load_flare_main();
        automap_fix_main();
        cheat_main();
        //devmode_main();
        fullgame_main();
        infraring_fix_main();
        mega_fix_main();
        music_nompq_fix_main();
        savegame_patch_fix_main();
        skip_intros_main();
        version_override_main();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
