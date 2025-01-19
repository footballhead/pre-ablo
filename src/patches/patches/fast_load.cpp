#include "patches.hpp"
#include "util.hpp"

#define PATCH_NAME fast_load
DESCRIBE_PATCH(
    "Loads faster during level transitions by removing the artificial wait to "
    "paint when incrementing the progress bar.")

PATCH_MAIN {
  // IncProgress: remove wait tor paint during fade. Fades will happen
  // instantly.
  bool ok = nop(0x00419BA6, 0x00419BB3);
  // IncProgress: remove wait to paint while incrementing progress bar. It will
  // likely instantly fill.
  ok = nop(0x00419C19, 0x00419C26);
  return ok;
}
