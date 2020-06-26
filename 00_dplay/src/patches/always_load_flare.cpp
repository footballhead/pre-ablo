#include "patches.hpp"

#include "functions.hpp"
#include "util.hpp"

namespace {

//
// Imports
//

auto** const pMagballCels = reinterpret_cast<BYTE**>(0x005F6F90); // array of pointers
auto** const pThinLghningCel = reinterpret_cast<BYTE**>(0x005FC0A0); // pointer of pointers
auto** const pFlareCel = reinterpret_cast<BYTE**>(0x005F71E0); // pointer to pointer
auto** const pFlarexpCel = reinterpret_cast<BYTE**>(0x005FC89C); // pointer to pointer

//
// Locals
//

bool magball_loaded = false;

//
// Hooks
//

void load_magball()
{
    // The real reason we're here: early return if already loaded
    if (magball_loaded) {
        return;
    }
    magball_loaded = true;
    // Displaced code
    char buffer[256]{'\0'};
    for (auto i = 0; i < 8; ++i) {
        snprintf(buffer, 256, "Monsters\\Magma\\Magball%i.CEL", i + 1);
        pMagballCels[i] = LoadFileInMem(buffer);
    }
}

void free_magball()
{
    // The real reason we're here: early return if already freed
    if (!magball_loaded) {
        return;
    }
    magball_loaded = false;
    // displaced code
    for (auto i = 0; i < 8; ++i) {
        GlobalUnlock(GlobalHandle(pMagballCels[i]));
        GlobalFree(GlobalHandle(pMagballCels[i]));
        pMagballCels[i] = nullptr;
    }
}

void load_thin_lightning()
{
    // The real reason we're here: early return if already loaded
    if (*pThinLghningCel) {
        return;
    }
    // displaced code
    *pThinLghningCel = LoadFileInMem("Monsters\\Thin\\Lghning.CEL");
}

void free_thin_lightning()
{
    // The real reason we're here: early return if already freed
    if (!*pThinLghningCel) {
        return;
    }
    // displaced code
    GlobalUnlock(GlobalHandle(*pThinLghningCel));
    GlobalFree(GlobalHandle(*pThinLghningCel));
    *pThinLghningCel = nullptr;
}

void load_flare_gfx()
{
    // The real reason we're here: early return if already loaded
    if (*pFlareCel) {
        return;
    }
    // displaced code
    *pFlareCel = LoadFileInMem("Monsters\\Succ\\Flare.CEL");
    *pFlarexpCel = LoadFileInMem("Monsters\\Succ\\Flarexp.CEL");
}

void free_flare_gfx()
{
    // The real reason we're here: early return if already freed
    if (!*pFlareCel) {
        return;
    }
    // displaced code
    GlobalUnlock(GlobalHandle(*pFlareCel));
    GlobalFree(GlobalHandle(*pFlareCel));
    *pFlareCel = nullptr;
    GlobalUnlock(GlobalHandle(*pFlarexpCel));
    GlobalFree(GlobalHandle(*pFlarexpCel));
    *pFlarexpCel = nullptr;
}

} // namespace

bool always_load_flare_main()
{
    bool ok = true;

    // In each case we keep the mtype check but redirect loading/freeing to our own code.
    // To hook, we first nop the problem code, then we patch in a call to our fixed code.

    // Load magball
    ok &= nop(0x004018F4, 0x00401940);
    ok &= patch_call(0x004018FA, (void*)load_magball);

    // Free magball
    ok &= nop(0x0040B78B, 0x0040B7CD);
    ok &= patch_call(0x0040B78E, (void*)free_magball);

    // Load thin lightning
    ok &= nop(0x0040195A, 0x00401969);
    ok &= patch_call(0x0040195A, (void*)load_thin_lightning);
    
    // Free thin lightning
    ok &= nop(0x0040B7E1, 0x0040B807);
    ok &= patch_call(0x0040B7E1, (void*)free_thin_lightning);

    // load flare
    ok &= nop(0x00401983, 0x004019A1);
    ok &= patch_call(0x00401983, (void*)load_flare_gfx);

    // Free flare
    ok &= nop(0x0040B81B, 0x0040B867);
    ok &= patch_call(0x0040B81B, (void*)free_flare_gfx);

    return ok;
}
