#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME magic_find_x3
DESCRIBE_PATCH(R"txt(Increase magic find 3 fold)txt")

PATCH_MAIN
{
    bool ok = true;

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

    return ok;
}

// others:
// randomize/shuffle unique item properties
