#include "functions.hpp"
#include "patches.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME unimplemented_spells_fix
DESCRIBE_PATCH(
    "Unimplemented spells like Invisibility, Etherealize, and Golem will fully "
    "act like Stone Curse.")

PATCH_MAIN {
  // ProcessMissiles: Treat missiles 32-35 as stone curse. Otherwise, the
  // missiles will never be freed and the game will run out of missiles.
  // This compliments both AddMissile (which calls AddStone for these missiles
  // in the first place), and SetMissileGFX (which sets the anim data on load).
  bool ok = patch_dword(0x004429A6, 0x004428F3);
  ok &= patch_dword(0x004429AA, 0x004428F3);
  ok &= patch_dword(0x004429AE, 0x004428F3);
  ok &= patch_dword(0x004429B2, 0x004428F3);
  return ok;
}
