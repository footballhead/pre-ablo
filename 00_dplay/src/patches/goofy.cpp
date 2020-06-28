#include "patches.hpp"

#include "util.hpp"

#include <cstdlib>

#define PATCH_NAME goofy
DESCRIBE_PATCH(R"txt(Buncha goofy stuff to shake things up

- Remove monstimgtot check to allow up to 16 monster types per dungeon level
- Higher magic find
- Increased monster density)txt")

namespace {

//
// imports
//

auto const CheckUnique = reinterpret_cast<int (__fastcall *)(int, int, int)>(0x004216EB);

// // Return a random dungeon level type.
// // Does not work well with object loading so crashes a lot :(
// int __fastcall InitLevelType_hook(int l)
// {
//     return l == 0 ? 0 : rand() % 5 + 1;
// }

int __fastcall CheckUnique_hook(int i, int lvl, int uper)
{
    return CheckUnique(i, lvl, uper);
}

} // nameespace

PATCH_MAIN
{
    bool ok = true;

    // // Patch InitLevelType to return random levels
    // ok &= patch_jmp(0x00488442, (void*)InitLevelType_hook);

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

    return ok;
}

// others:
// randomize level types (would this mess with quests?)
// randomize/shuffle unique item properties
