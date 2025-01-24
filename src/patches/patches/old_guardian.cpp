#include "patches.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME old_guardian
DESCRIBE_PATCH(
    "Replaces Guardian (which is otherwise a copy of Sentinal) with an unused "
    "protoype.")

namespace {

#define MIS_GUARDIAN 2
#define MIS_SENTINAL 15

using AddMissilePrototype = void(__fastcall*)(int mi, int sx, int sy, int dx,
                                              int dy, int midir, int micaster,
                                              int id, int midam);
using MissileLogicPrototype = void(__fastcall*)(int i);

AddMissilePrototype AddSentinal =
    reinterpret_cast<AddMissilePrototype>(0x004389BB);
AddMissilePrototype AddGuardian_Old =
    reinterpret_cast<AddMissilePrototype>(0x0043A2DF);
MissileLogicPrototype MI_Sentinal =
    reinterpret_cast<MissileLogicPrototype>(0x0043FBE4);
MissileLogicPrototype MI_Guardian_Old =
    reinterpret_cast<MissileLogicPrototype>(0x004416F7);

void __fastcall AddSentinal_Wrapper(int mi, int sx, int sy, int dx, int dy,
                                    int midir, int micaster, int id,
                                    int midam) {
  switch (missile[mi]._mitype) {
    case MIS_GUARDIAN:
      // Call the prototype code
      AddGuardian_Old(mi, sx, sy, dx, dy, midir, micaster, id, midam);
      break;
    case MIS_SENTINAL:
      // Misplaced code
      AddSentinal(mi, sx, sy, dx, dy, midir, micaster, id, midam);
      break;
  }
}

void __fastcall MI_Sentinal_Wrapper(int i) {
  switch (missile[i]._mitype) {
    case MIS_GUARDIAN:
      // Call the prototype code
      MI_Guardian_Old(i);
      break;
    case MIS_SENTINAL:
      // Misplaced code
      MI_Sentinal(i);
      break;
  }
}

}  // namespace

PATCH_MAIN {
  // AddMissile: Replace original call to AddSentinal with our own. This allows
  // us to discriminate between MIS_GUARDIAN and MIS_SENTINAL
  bool ok = patch_call(0x0043B0B7, AddSentinal_Wrapper);
  // ProcessMissiles: Replace original call to MI_Sentinal with our own. This
  // allos us to discriminate between MIS_GUARDIAN and MIS_SENTINAL
  ok &= patch_call(0x00442881, MI_Sentinal_Wrapper);
  return ok;
}
