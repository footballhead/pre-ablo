#include "functions.hpp"
#include "patches.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME blood_boil_fix
DESCRIBE_PATCH("Fixes Blood Boil not working on some monsters.")

namespace {

void __fastcall cast_bloodboil_wrapper(int id, int si, int sx, int sy, int dx,
                                       int dy) {
  // No monster at cursor tile, don't bother. This effectively replaces the
  // `if (mid > 0)` check inside the original function. See
  // https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/issues/114 for
  // more info.
  if (dMonster[dx][dy] == 0) {
    // This is at the bottom of cast_bloodboil and would always run. We now
    // take over the responsibility of deleting the spell object when the cast
    // fails.
    spells[si].delFlag = TRUE;
    return;
  }
  // Otherwise, proceed with cast.
  cast_bloodboil(id, si, sx, sy, dx, dy);
}

}  // namespace

PATCH_MAIN {
  // CastSpell: Replace original function with our modifications
  bool ok = patch_call(0x0045631C, cast_bloodboil_wrapper);
  // cast_bloodboil: Remove the original broke `mid` check in favor of ours
  ok &= nop(0x00455507, 0x00455511);
  return ok;
}
