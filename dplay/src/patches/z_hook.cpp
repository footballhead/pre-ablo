#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME z_hook
DESCRIBE_PATCH("Do something when Z is pressed")

namespace {

// auto const SpawnUnique = reinterpret_cast<void (__fastcall *)(int, int, int)>(0x00421CDC);

// auto const plr_px = reinterpret_cast<DWORD* const>(0x006058DC);
// auto const plr_py = reinterpret_cast<DWORD* const>(0x006058E0);

void __fastcall z_press_hook()
{
    // 57 - Constricting ring
    // SpawnUnique(57, *plr_px, *plr_py);

    // Use old sentinel creation code
    // patch_call(0x0043B0B7, (void*)0x0043A2DF);
    // Use old sentinel tick code (follows player, shoots fireballs)
    // patch_call(0x00442881, (void*)0x004416F7);

    // This makes the missile follow the player a few tiles up
    // If it collides with a wall then the game hangs :(
    patch_call(0x0044289B, (void*)0x00441F56);
    nop(0x0044288E, 0x00442893);

    // idk what this does
    // patch_call(0x004576E6, (void*)0x00456CA4);

    
}

} // namespace

PATCH_MAIN
{
    bool ok = true;

    ok &= nop(0x0048722B, 0x00487251);

    ok &= patch_call(0x0048722B, (void*)z_press_hook);

    return ok;
}

