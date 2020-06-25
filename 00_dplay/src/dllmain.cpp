#include "patches.hpp"

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
        // TODO: redirect to file #ifdef NDEBUG?

        for (auto const patch : get_patches()) {
            if (patch.checked) {
                printf("%s...\n", patch.name);
                auto ok = patch.main();
                printf("%s: %s\n", patch.name, ok ? "success" : "fail");
            }
        }

        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
