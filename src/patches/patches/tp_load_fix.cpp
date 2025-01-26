#include "patches.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME tp_load_fix
DESCRIBE_PATCH(
    "Prevents crashing when loading with a town portal by setting the correct "
    "graphic.")

namespace {

BYTE** const pPortalCel = reinterpret_cast<BYTE** const>(0x005FC688);
BYTE** const pPortalUCel = reinterpret_cast<BYTE** const>(0x005F7260);

void __fastcall SetMissileGfx_TownPortalFix(int i) {
  // _miVar1 is set by AddTown as the threshold for changing from opening to
  // opened portal. _miRange decrements by 1 each iteration, stopping at 1. We
  // can use these to check whether the portal should use the opening or already
  // open graphics.
  //
  // If we don't correct this then MI_Town might incorrectly think that we
  // should be playing the opening animation, which updates the lighting, which
  // will cause out of bounds memory lookup, and crash.
  if (missile[i]._mirange <= missile[i]._miVar1) {
    // Portal open
    missile[i]._miAnimData = *pPortalCel;
  } else {
    // Still opening
    missile[i]._miAnimData = *pPortalUCel;
  }
}

}  // namespace

PATCH_MAIN {
  // SetMissileGFX: Like stone_curse_load_fix, keep mi in ECX but nop the rest.
  bool ok = nop(0x0044320F, 0x0044321B);
  // SetMissileGFX: Call our patch to determine which portal graphic to load.
  ok &= patch_call(0x0044320F, SetMissileGfx_TownPortalFix);
  return ok;
}
