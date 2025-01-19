#include "functions.hpp"
#include "patches.hpp"
#include "util.hpp"

#define PATCH_NAME remove_promo
DESCRIBE_PATCH("Removes the promotional slideshow displayed when quitting.")

void __fastcall JustQuitAlready() {
  // Rely on the OS to clean up memory, etc
  ExitProcess(0);
}

PATCH_MAIN {
  // interfac_InitDemoEnd is called when the game wants to show the slideshow.
  // It handles loading the slides. Replacing the body should handle all cases
  // where the slideshow would be shown.
  // interfac_InitDemoEnd: nop existing code; replace with ExitProcess
  bool ok = nop(0x0041BECA, 0x0041BF73);
  ok &= patch_call(0x0041BECA, JustQuitAlready);
  return true;
}
