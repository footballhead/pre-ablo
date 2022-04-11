#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME plrgfx_frame_fix
DESCRIBE_PATCH("Adjust death animations to match the files we have.")

typedef enum anim_weapon_id {
    ANIM_ID_UNARMED        = 0x00,
    ANIM_ID_UNARMED_SHIELD = 0x01,
    ANIM_ID_SWORD          = 0x02,
    ANIM_ID_SWORD_SHIELD   = 0x03,
    ANIM_ID_BOW            = 0x04,
    ANIM_ID_AXE            = 0x05,
    ANIM_ID_MACE           = 0x06,
    ANIM_ID_MACE_SHIELD    = 0x07,
    ANIM_ID_STAFF          = 0x08
} anim_weapon_id;

typedef enum anim_armor_id {
    ANIM_ID_LIGHT_ARMOR = 0x00,
    ANIM_ID_MEDIUM_ARMOR = 0x10,
    ANIM_ID_HEAVY_ARMOR  = 0x20
} anim_armor_id;

int __fastcall get_warrior_DFrames(int gfxnum) {
    // gfxnum is anim_weapon_id | anim_armor_id
    switch (gfxnum) {
    case ANIM_ID_LIGHT_ARMOR | ANIM_ID_UNARMED:
        return 20; // .text:004668B3
    case ANIM_ID_LIGHT_ARMOR | ANIM_ID_UNARMED_SHIELD:
        return 20; // .text:0046691D
    case ANIM_ID_LIGHT_ARMOR | ANIM_ID_SWORD:
        return 21; // .text:00466987
    case ANIM_ID_LIGHT_ARMOR | ANIM_ID_SWORD_SHIELD:
        return 21; // .text:0046688A
    case ANIM_ID_LIGHT_ARMOR | ANIM_ID_BOW:
        // This is never overriden so it'll be 15. However, we know after
        // looking wlbdt that this should be 21.
        return 21;
    case ANIM_ID_LIGHT_ARMOR | ANIM_ID_AXE:
        // Never overriden but wladt is 15 frames
        return 15;
    case ANIM_ID_LIGHT_ARMOR | ANIM_ID_MACE:
        return 16; // .text:00466AAD
    case ANIM_ID_LIGHT_ARMOR | ANIM_ID_MACE_SHIELD:
        return 16; // .text:00466AF8
    case ANIM_ID_LIGHT_ARMOR | ANIM_ID_STAFF:
        return 20; // .text:00466B5F
    }

    // We only check light armor because those are the only GFX we have. From
    // beta we only have whndt and wmndt and both are 15 frames.
    return 15;
}

PATCH_MAIN
{
    bool ok = true;

    // TODO: We could do this in C++ if we knew the approximate size and layout of PlayerStruct...

    // Remove existing code that sets death frames
    ok &= nop(0x00466853, 0x004669D2);
    ok &= nop(0x00466A8E, 0x00466B02);
    ok &= nop(0x00466B4A, 0x00466B69);

    // Call our function and store the result.
    // Inspiration from .text:0041544D
    constexpr uint8_t pre_call[] = {
        0x8B, 0x45, 0xF4,                   // mov     eax, [ebp+pnum]
        0x8B, 0xC8,                         // mov     ecx, eax
        0xC1, 0xE0, 0x05,                   // shl     eax, 5
        0x03, 0xC1,                         // add     eax, ecx
        0x8D, 0x04, 0x81,                   // lea     eax, [ecx+eax*4]
        0xC1, 0xE0, 0x03,                   // shl     eax, 3
        0x2B, 0xC1,                         // sub     eax, ecx
        0xC1, 0xE0, 0x04,                   // shl     eax, 4
        0x8B, 0x88, 0x04, 0x59, 0x60, 0x00, // mov     ecx, plr_pgfxnum[eax]
    };
    ok &= patch_bytes(0x00466853, pre_call, sizeof(pre_call));
    ok &= patch_call(0x00466853 + sizeof(pre_call), (void*)get_warrior_DFrames);
    constexpr auto CALL_SIZE = 5;
    // Inspiration from .text:0041544D (I could probably move edx, ecx but I'm not good at machine code lol)
    constexpr uint8_t post_call[] = {
        0x8B, 0x4D, 0xF4,                   // mov     ecx, [ebp+pnum]
        0x8B, 0xD1,                         // mov     edx, ecx
        0xC1, 0xE1, 0x05,                   // shl     ecx, 5
        0x03, 0xCA,                         // add     ecx, edx
        0x8D, 0x0C, 0x8A,                   // lea     ecx, [edx+ecx*4]
        0xC1, 0xE1, 0x03,                   // shl     ecx, 3
        0x2B, 0xCA,                         // sub     ecx, edx
        0xC1, 0xE1, 0x04,                   // shl     ecx, 4
        0x89, 0x81, 0xD8, 0x5C, 0x60, 0x00, // mov     plr_pDFrames[ecx], eax
    };
    ok &= patch_bytes(0x00466853 + sizeof(pre_call) + CALL_SIZE, post_call, sizeof(post_call));

    // The one rogue death animation that we have from beta only has 20 frames
    // Old:
    //   .text:00466C18 C7 80 D8 5C 60 00 15 00 00 00                 mov     plr_pDFrames[eax], 21
    // New
    //   .text:00466C18 C7 80 D8 5C 60 00 14 00 00 00                 mov     plr_pDFrames[eax], 20
    uint8_t rogue_death_frames = 20;
    ok &= patch_bytes(0x00466C18 + 6, &rogue_death_frames, 1);

    return ok;
}
