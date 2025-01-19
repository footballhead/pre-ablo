#include <windows.h>

#include "patches.hpp"
#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME performance_improvements
DESCRIBE_PATCH(
    R"txt(- Improves load times during level transitions by removing the artificial wait to paint when incrementing the progress bar. This might be even faster if VSYNC is disabled!

- Reworks game rendering logic to use less CPU, improve smoothness, and fix bugs.

Consider disabling this if rendering is janky.)txt")

namespace {

#define WM_DIABPAINT WM_USER

constexpr auto kOneSecondInMilliseconds = 1000;
constexpr auto kDiabloFps = 20;
constexpr auto kFrameIntervalMs = kOneSecondInMilliseconds / kDiabloFps;

void __fastcall MainLoopYield() {
  // This variable is normally incremented every time that PaintEventTimer is
  // fired. This is useful diagnostic information. Without patches, this hovers
  // around 200 since it fires every 5ms (see dx_init). However, we don't use
  // PaintEventTimer anymore; the equivalent is this function. We continue to
  // update this since it is printed when F4 is pressed (with cheats on).
  ++main_loop_iteration_count;

  // Dispatch paint event every 50ms. This should happen both in-game and on the
  // main menu. Ultimately, this sidesteps the odd mulithreading
  // outstanding_paint_event mechanism which is prone to get stuck (due to lack
  // of proper critical sections).
  // TODO what happens on lower end computers that can't keep up?
  DWORD now = timeGetTime();
  if (now - prev_timer_PostMessage_time >= kFrameIntervalMs) {
    PostMessage(ghMainWnd, WM_DIABPAINT, 0, 0);
    prev_timer_PostMessage_time = now;
    // TODO respect other vars like outstanding_paint_event, etc?
  }

  // Reset FPS statistics every second (displaced from PaintEventTimer).
  // This shows up with cheats when F4 is pressed.
  if (now - prevTime >= kOneSecondInMilliseconds) {
    prevTime = now;
    last_fps = frames;
    frames = 0;
    last_main_loop_iteration_count = main_loop_iteration_count;
    main_loop_iteration_count = 0;
  }

  // Yield to OS to prevent hogging CPU.
  // Looking at Task Manager, Sleep(0) doesn't achieve our goals. There's also
  // advice to always Sleep(1) for reasons around thread priorities:
  // https://joeduffyblog.com/2006/08/22/priorityinduced-starvation-why-sleep1-is-better-than-sleep0-and-the-windows-balance-set-manager/
  Sleep(1);
}

}  // namespace

PATCH_MAIN {
  // IncProgress: remove wait for paint during fade. Fades will happen
  // instantly.
  bool ok = nop(0x00419BA6, 0x00419BB3);
  // IncProgress: remove wait to paint while incrementing progress bar. It will
  // likely instantly fill.
  ok &= nop(0x00419C19, 0x00419C26);

  // WinMain: in the main loop, combine the continue with the break. The main
  // loop never breaks so this should be fine.
  ok &= nop(0x00484E53, 0x00484E60);
  // WinMain: for main loop continue, inject our own function to sleep. Restore
  // the jmp to the top of the loop after our function returns.
  ok &= patch_call(0x00484E53, MainLoopYield);
  ok &= patch_jmp(0x00484E53 + kCallInstructionSize, 0x00484CC3);
  // dx_init: Remove PaintEventTimer registration (in favor of our own logic).
  ok &= nop(0x00485336, 0x0048535B);
  return ok;
}
