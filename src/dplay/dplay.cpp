#include "patches/patches.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdio>

extern "C" BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD ul_reason_for_call, LPVOID /*lpReserved*/)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // Theoretically, we shouldn't do initialization here. Instead, it should be done at the top of DIABLO.EXE main().
        // This is a well known limitation of DllMain spelled out in the docs:
        // https://docs.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-best-practices
        // I think we get away with it because dplay.dll is loaded after the DLLs on which we depend.

#ifndef NDEBUG
        AllocConsole();
        freopen("CONOUT$", "wb", stdout);
#endif
        // TODO: redirect to file #ifdef NDEBUG?

        for (auto const& patch : get_patches()) {
            if (patch.checked && patch.main) {
                printf("%s...\n", patch.name);
                auto ok = patch.main();
                printf("%s: %s\n", patch.name, ok ? "success" : "fail");
            }
        }

        // Patch MPQ loading is done shortly after main(). See `load_mpq_customizations` patch.

        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
