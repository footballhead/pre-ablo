#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME z_hook
DESCRIBE_PATCH("Do something when Z is pressed")

namespace {

auto const SpawnUnique = reinterpret_cast<void (__fastcall *)(int, int, int)>(0x00421CDC);

auto const plr_px = reinterpret_cast<DWORD* const>(0x006058DC);
auto const plr_py = reinterpret_cast<DWORD* const>(0x006058E0);

void __fastcall z_press_hook()
{
    // 57 - Constricting ring
    SpawnUnique(57, *plr_px, *plr_py);
}

} // namespace

PATCH_MAIN
{
    bool ok = true;

    ok &= nop(0x0048722B, 0x00487251);

    ok &= patch_call(0x0048722B, (void*)z_press_hook);

    return ok;
}

