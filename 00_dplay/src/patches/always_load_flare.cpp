#include "patches.hpp"

#include "util.hpp"

namespace {

//
// Imports
//

auto** const pMagballCels = reinterpret_cast<BYTE**>(0x005F6F90); // array of pointers
auto** const pThinLghningCel = reinterpret_cast<BYTE**>(0x005FC0A0); // pointer of pointers
auto** const pFlareCel = reinterpret_cast<BYTE**>(0x005F71E0); // pointer to pointer
auto** const pFlarexpCel = reinterpret_cast<BYTE**>(0x005FC89C); // pointer to pointer

auto LoadFileInMem = reinterpret_cast<BYTE* (__fastcall *)(const char* pszName)>(0x00482790);

//
// Locals
//

bool loaded = false;

//
// Hooks
//

void load_flare_gfx()
{
    if (loaded) {
        return;
    }

    // Could also reuse the string literals but meh this is more readable
    char buffer[256]{'\0'};
    for (auto i = 1; i <= 8; ++i) {
        snprintf(buffer, 256, "Monsters\\Magma\\Magball%i.CEL", i);
        pMagballCels[i] = LoadFileInMem(buffer);
    }
    *pThinLghningCel = LoadFileInMem("Monsters\\Thin\\Lghning.CEL");
    *pFlareCel = LoadFileInMem("Monsters\\Succ\\Flare.CEL");
    *pFlarexpCel = LoadFileInMem("Monsters\\Succ\\Flarexp.CEL");

    loaded = true;
}

void free_flare_gfx()
{
    if (!loaded) {
        return;
    }

    for (auto i = 0; i < 8; ++i) {
        GlobalUnlock(GlobalHandle(pMagballCels[i]));
        GlobalFree(GlobalHandle(pMagballCels[i]));
        pMagballCels[i] = nullptr;
    }
    GlobalUnlock(GlobalHandle(*pThinLghningCel));
    GlobalFree(GlobalHandle(*pThinLghningCel));
    *pThinLghningCel = nullptr;
    GlobalUnlock(GlobalHandle(*pFlareCel));
    GlobalFree(GlobalHandle(*pFlareCel));
    *pFlareCel = nullptr;
    GlobalUnlock(GlobalHandle(*pFlarexpCel));
    GlobalFree(GlobalHandle(*pFlarexpCel));
    *pFlarexpCel = nullptr;

    loaded = false;
}

} // namespace

void always_load_flare_main()
{
    //
    // Load
    //

    // Nop the code that already exists
    nop(0x004018CB, 0x004019A1);
    // Hook our load function (note that this is in a for loop so the hooked fn could be called many times)
    patch_call(0x004018CB, (void*)load_flare_gfx);

    //
    // Free
    //

    // Nop the code that already exists
    nop(0x0040B76B, 0x0040B867);
    // Hook our free function (same for loop note applies)
    patch_call(0x0040B76B, (void*)free_flare_gfx);
}
