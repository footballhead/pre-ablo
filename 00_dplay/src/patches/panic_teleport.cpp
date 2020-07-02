#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME panic_teleport
DESCRIBE_PATCH("Give new players the Teleport ability so they can get unstuck in otherwise bad level generations.")

namespace {

auto plr_pAblSpells = reinterpret_cast<uint32_t* const>(0x0060595C); // this is actually a bitfield
auto const CreatePlayer = reinterpret_cast<void (__fastcall *)(int, int)>(0x00466C74);

void __fastcall CreatePlayer_hook(int plr, int cls)
{
    CreatePlayer(plr, cls);
    // Give teleport ability. This is gold and doesn't cost mana.
    *plr_pAblSpells |= (1 << 22);
    // TODO: Fix the (null) skillname
}

} // namespace

PATCH_MAIN
{
    bool ok = true;

    // Give new players panic teleport
    ok &= patch_call(0x0041A722, (void*)CreatePlayer_hook);
    ok &= patch_call(0x0041A8B5, (void*)CreatePlayer_hook);

    return ok;
}
