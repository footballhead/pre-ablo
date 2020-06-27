#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME cheat
DESCRIBE_PATCH(R"txt(Turn on cheats. This has the following effects:

- You cannot be hit
- You have infinite mana
- You always hit monsters and kill them in one hit
- All drops are magic or unique

It also unlocks the following controls:

- F5: Teleport to starting house in town
- F6: Teleport to level 1
- F7: Teleport to level 5
- F8: Teleport to level 9
- * (Shift+8, or numpad): gain level
- Shift (hold): Allows RTS-style view scrolling when the mouse is near the edge of the screen
- L: Toggle lighting
- T: Show (x,y) coords on control panel
- D: Show transval value on control panel
- M: Show invincible value on control panel
- B: Increase current spell by 1 level)txt")

PATCH_MAIN
{
    // This is the data segment, no need to adjust permissions
    *(uint32_t*)0x004BC168 = TRUE;
    return true;
}
