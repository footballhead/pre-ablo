#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

#include <cstdlib>

#define PATCH_NAME goofy
DESCRIBE_PATCH(R"txt(Buncha goofy stuff to shake things up

- Remove monstimgtot check to allow up to 16 monster types per dungeon level
- Higher magic find
- Increased monster density)txt")

namespace {

auto ObjMasterLoadList = reinterpret_cast<char const* const* const>(0x004AC158);
auto plr_pAblSpells = reinterpret_cast<uint32_t* const>(0x0060595C); // this is actually a bitfield
auto const CreatePlayer = reinterpret_cast<void (__fastcall *)(int, int)>(0x00466C74);

void __fastcall CreatePlayer_hook(int plr, int cls)
{
    CreatePlayer(plr, cls);
    // Give panic teleport
    *plr_pAblSpells |= (1 << 22);
}

// Return a random dungeon level type.
int __fastcall InitLevelType_hook(int l)
{
    // TODO: Do I need to srand?
    // Keep some level types so quests work
    // TODO disable quests altogether?
    switch (l) {
    case 0: // town
        return 0; // make sure it stays town
    case 1: // level containing butcher
        return rand() % 2 ? 5 : 1; // must be one of the cathedrals
    case 2: // level 2 doesn't contain a quest but make sure it's not a cathedral to shake things up
        return rand() % 3 + 2;
    case 3: // level containing skeleton king
        return 5; // new cathedral
    case 4: // level containing Tome
        return 5; // new cathedral
    case 7: // level containing bone chamber
        return 2; // catacombs;
    default:
        // Otherwise, randomize it!
        return rand() % 5 + 1;
    }
}

} // nameespace

PATCH_MAIN
{
    bool ok = true;

    // Patch InitLevelType to return random levels
    ok &= patch_jmp(0x00488442, (void*)InitLevelType_hook);

    // nop monstimgtot checks
    // This means levels can have up to 16 monsters!
    ok &= nop(0x00401366, 0x00401373); // cmp and jge
    ok &= nop(0x004013A4, 0x004013AF); // cmp and jg

    // 100% chance to drop an item from monster kill
    ok &= nop(0x0042121E, 0x00421255);
    // 50% chance to drop gold from monster kill
    ok &= patch<uint8_t>(0x00421312 + 3, 50);
    // Flat 31% chance of magic item from monster kill
    ok &= patch<uint8_t>(0x00421ED4 + 2, 30);

    // Flat 31% chance of magic item from chests/barrels/sarc
    ok &= patch<uint8_t>(0x00422205 + 2, 30);
    // barrels have own logic for whether to drop an item, which I've left untouched

    // Flat 31% chance of magic item from treasure rooms
    ok &= patch<uint8_t>(0x00422562 + 2, 30);

    // Multiply unique chance by 3 for all drop cases (monster, chest, etc.)
    // Remove debug/cheat mode code up to dice rolling to make room for our patch
    ok &= nop(0x00421703, 0x0042171D);
    constexpr uint8_t uper_times_3[] = {
        0x8B, 0x45, 0x08, // mov eax, dword ptr[EBP+uper]
        0x03, 0xC0, // add eax, eax
        0x03, 0xC0, // add eax, eax
        0x89, 0x45, 0x08, // mov dword ptr[EBP+uper], eax
    };
    ok &= patch_bytes(0x00421703, uper_times_3, sizeof(uper_times_3));

    // Double monster density
    ok &= patch<uint32_t>(0x004031DE + 1, 15);

    for (auto i = 0; AllObjects[i].oload != -1; ++i) {
        auto& obj = AllObjects[i];
        // Allow all objects to spawn
        obj.ominlvl = 1;
        obj.omaxlvl = 16;
    }

    // From old DRLG patch
    // 3 is skulpile, it must always load to avoid crash
    AllObjects[3].oload = 1; // OBJ_MUST
    AllObjects[3].otheme = -1;

    // Give player panic teleport
    ok &= patch_call(0x0041A722, (void*)CreatePlayer_hook);
    ok &= patch_call(0x0041A8B5, (void*)CreatePlayer_hook);

    return ok;
}

// others:
// randomize/shuffle unique item properties
