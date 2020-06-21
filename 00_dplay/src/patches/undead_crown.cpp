#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

namespace {

auto* const sfxdelay = reinterpret_cast<int* const>(0x0060535C);
auto* const sfxdnum = reinterpret_cast<int* const>(0x00604508);

// void __fastcall CreateItem(int uid, int x, int y)
auto const CreateItem = reinterpret_cast<void (__fastcall *)(int, int, int)>(0x00422056);

void __fastcall onSkelKingDeath(int m)
{
    // displaced code
    quests[0]._qactive = 3;
    *sfxdelay = 30;
    *sfxdnum = 56;

    // new code
    CreateItem(8, monster[m]._mx, monster[m]._my);
}

} // namespace

void undead_crown_main()
{
    // Disable broken drop entry (so it doesn't drop the rift bow)
    // This technically means the skeleton king drops 2 things (the undead crown and a random drop)
    monsterdata[48].mTreasure = 0;

    // Patch CheckQuestKill for sking death
    nop(0x004719FB, 0x00471A16); // nop existing code for a blank canvas
    constexpr uint8_t mov_ecx_m[] = {0x8B, 0x4D, 0xFC};
    patch_bytes(0x004719FB, mov_ecx_m, sizeof(mov_ecx_m)); // m is first param for onSkelKingDeath, __fastcall means put it in ECX
    patch_call(0x004719FB + sizeof(mov_ecx_m), (void*)onSkelKingDeath); // do onSkelKingDeath
}
