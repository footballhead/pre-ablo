#include "patches.hpp"

#include "util.hpp"

#include <cstdlib>

#define PATCH_NAME z_hook
DESCRIBE_PATCH("Do something when Z is pressed")

namespace {

uint32_t* const plr_pdir = reinterpret_cast<uint32_t*>(0x00605478 + 0x484);
uint32_t* const plr_pgfxnum = reinterpret_cast<uint32_t*>(0x00605478 + 0x48C);
BOOL* const plr_pBlockFlag = reinterpret_cast<BOOL*>(0x00605478 + 0x504);
uint32_t* const plr_pGFXLoad = reinterpret_cast<uint32_t*>(0x00605478 + 0x72C);
BYTE* const plr_pNAnim = reinterpret_cast<BYTE*>(0x00605478 + 0x730);
int* const myplr = reinterpret_cast<int* const>(0x00615E38);

typedef void (__fastcall *pfnInitPlayerGFX)(int);
const pfnInitPlayerGFX InitPlayerGFX = reinterpret_cast<pfnInitPlayerGFX>(0x00465887);

typedef void (__fastcall *pfnSetPlrAnims)(int);
const pfnSetPlrAnims SetPlrAnims = reinterpret_cast<pfnSetPlrAnims>(0x0046652F);

typedef void (__fastcall *pfnStartStand)(int, int);
const pfnStartStand StartStand = reinterpret_cast<pfnStartStand>(0x00468BF1);

constexpr uint32_t kMaxWeaponAnims = 9; // NUSDBAMHT
constexpr uint32_t kMaxArmorAnims = 3; // LMH

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

// Cycle through gfxnum weapons and armor
uint32_t NextAnimation(uint32_t gfxnum)
{
    uint32_t weapon = gfxnum & 0xF;
    uint32_t armor = gfxnum >> 4;
    ++weapon;
    if (weapon >= kMaxWeaponAnims) {
        weapon = 0;
        ++armor;
    }
    if (armor >= kMaxArmorAnims) {
        armor = 0;
    }
    return (armor << 4) | weapon;
}

uint32_t HasBlockAnimation(uint32_t gfxnum)
{
    switch (gfxnum & 0xF) {
    case ANIM_ID_UNARMED_SHIELD:
    case ANIM_ID_SWORD_SHIELD:
    case ANIM_ID_MACE_SHIELD:
        return true;
    default:
        return false;
    }
}

void __fastcall z_press_hook()
{
    // Cycle to next graphics.

    // Tell the algos which animations to load
    *plr_pgfxnum = NextAnimation(*plr_pgfxnum);
    *plr_pBlockFlag = HasBlockAnimation(*plr_pgfxnum);
    *plr_pGFXLoad = 0;

    // Load all animation CELs. Some things might be able to handle piece-meal
    // load but better to be safe than sorry
    InitPlayerGFX(*myplr);
    // Ensure animation lengths are set
    SetPlrAnims(*myplr);

    // Reuse StartStand state transition restart the animation.
    // This is basically what CalcPlrItemVals does anyway.
    StartStand(*myplr, *plr_pdir);
}

} // namespace

PATCH_MAIN
{
    bool ok = true;
    ok &= nop(0x0048722B, 0x00487251);
    ok &= patch_call(0x0048722B, (void*)z_press_hook);
    return ok;
}

