#include "patches.hpp"

#include "util.hpp"

namespace {

//
// Imports
//

auto pFlareCel = reinterpret_cast<BYTE**>(0x005F71E0);
auto pFlarexpCel = reinterpret_cast<BYTE**>(0x005FC89C);

auto LoadFileInMem = reinterpret_cast<BYTE* (__fastcall *)(const char* pszName)>(0x00482790);

//
// Hooks
//

void load_flare_gfx()
{
    if (*pFlareCel) {
        // Already loaded
        return;
    }
    // Could also reuse the string literals but meh this is more readable
    *pFlareCel = LoadFileInMem("Monsters\\Succ\\Flare.CEL");
    *pFlarexpCel = LoadFileInMem("Monsters\\Succ\\Flarexp.CEL");
}

void free_flare_gfx()
{
    if (*pFlareCel == nullptr) {
        // Already freed
        return;
    }
    GlobalUnlock(GlobalHandle(*pFlareCel));
    GlobalFree(GlobalHandle(*pFlareCel));
    *pFlareCel = nullptr;
    GlobalUnlock(GlobalHandle(*pFlarexpCel));
    GlobalFree(GlobalHandle(*pFlarexpCel));
    *pFlarexpCel = nullptr;
}

} // namespace

void always_load_flare_main()
{
    // Hook our load function (note that this is in a for loop so the hooked fn could be called many times)
    patch_call(0x00401969, (void*)load_flare_gfx);
    // Nop the code that already exists
    nop(0x00401969 + 5, 0x004019A1);

    // Hook our free function (same for loop note applies)
    patch_call(0x0040B807, (void*)free_flare_gfx);
    // Nop the code that already exists
    nop(0x0040B807 + 5, 0x0040B867);
}
