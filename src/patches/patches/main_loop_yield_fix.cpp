#include "patches.hpp"
#include "util.hpp"

#define PATCH_NAME main_loop_yield_fix
DESCRIBE_PATCH(
    "Stops the CPU from using 100% of a core by yielding to the OS during the "
    "main loop.")

namespace {

void __fastcall MainLoopYield() {
  // Looking at Task Manager, Sleep(0) doesn't achieve our goals. There's also
  // advice to always Sleep(1) for reasons around thread priorities:
  // https://joeduffyblog.com/2006/08/22/priorityinduced-starvation-why-sleep1-is-better-than-sleep0-and-the-windows-balance-set-manager/
  Sleep(1);
}

}  // namespace

PATCH_MAIN {
  // WinMain: in the main loop, combine the continue with the break. The main
  // loop never breaks so this should be fine.
  bool ok = nop(0x00484E53, 0x00484E60);
  // WinMain: for main loop continue, inject our own function to sleep. Restore
  // the jmp to the top of the loop after our function returns.
  ok = patch_call(0x00484E53, MainLoopYield);
  ok = patch_jmp(0x00484E53 + kCallInstructionSize, 0x00484CC3);
  // dx_init: Fire PaintEventTimer more often, every 1ms instead of 5ms. The
  // increased frequency of paint allows the code as written to more reliably
  // hit frame rate targets.
  ok &= patch_byte(0x004852F0 + 2, 1);
  ok &= patch_dword(0x004852F9 + 1, 1);
  return ok;
}
