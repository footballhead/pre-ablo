#include "enums.hpp"
#include "functions.hpp"
#include "patches.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME stone_curse_load_fix
DESCRIBE_PATCH(
    "Prevents crashing when loading a game with a Stone Cursed monster.")

namespace {

void __fastcall SetMissileGfx_StoneCurseFix(int i) {
  // If the animation data was explicitly set to NULL then the monster hasn't
  // been shattered yet. We can't set the animdata to pShatter1Cel in this case
  // because that would cause MI_Stone to make it visible when it shouldn't be
  // and crash. We only need to fix serialized pointers that would otherwise
  // point to garbage memory.
  if (missile[i]._miAnimData != NULL) {
    missile[i]._miAnimData = *pShatter1Cel;
  }
}

// Not sure what to do with this information so I'll put it here. Retail fixes
// this issue by adding an additional flag to MissileStruct called
// `_miDrawFlag`. The missile will only be drawn if this is set to TRUE. This
// solves at least two bugs that I'm aware of (see stone_curse_missile_fix,
// this one), maybe more. _miDrawFlag gives the programmer more explicit control
// over whether the missile should be visible, rather than making incorrect
// assumptions based on previous state.

// Wrap the function so I have easy access to the missile index.
void __fastcall SyncMonsterAnim_wrapper(int i) {
  // Run the original function so that MType, MData, etc are set
  SyncMonsterAnim(i);

  // Handle the missing cases
  int _mdir = monster[i]._mdir;
  switch (monster[i]._mmode) {
    case MM_STONE:
      monster[i]._mAnimData = monster[i].MType->Anims[MA_STAND].Data[_mdir];
      monster[i]._mAnimFrame = 1;
      monster[i]._mAnimLen = monster[i].MType->Anims[MA_STAND].Frames;
    case MM_CHARGE:
      monster[i]._mAnimData = monster[i].MType->Anims[MA_ATTACK].Data[_mdir];
      monster[i]._mAnimFrame = 1;
      monster[i]._mAnimLen = monster[i].MType->Anims[MA_ATTACK].Frames;
  }
}

}  // namespace

PATCH_MAIN {
  bool ok = true;

  // LoadLevel: Intercept the call to SyncMonsterAnim so that we can augment it
  // with the missing cases like MM_STONE
  ok &= patch_call(0x00460B1C, (void*)SyncMonsterAnim_wrapper);
  // TODO: There are other callers of SyncMonsterAnim but I think that they're
  // all dead code

  // SetMissileGfx: Keep the code that moves the missile index into ECX for
  // later use, but remove the conversion to a byte offset from missiles
  // .text:00443356     mov     eax, pNewexpCel               ; No harm to keep
  // .text:0044335B     mov     ecx, [ebp+mi]                 ; Keep
  // .text:0044335E     lea     ecx, [ecx+ecx*4]              : NOP
  // .text:00443361     shl     ecx, 5                        ; NOP
  // .text:00443364     mov     missile._miAnimData[ecx], eax ; NOP then patch
  ok &= nop(0x00443345, 0x0044336A);
  // ECX (the missile index) becomes the first __fastcall function parameter
  ok &= patch_call(0x00443364, (void*)SetMissileGfx_StoneCurseFix);

  return ok;
}
