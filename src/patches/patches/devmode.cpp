#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME devmode
DESCRIBE_PATCH(R"txt(Enables a developer debug mode. This does the following:

- New games start on dlvl 1
- All monsters are Corpse Axes (except for setlevels and the Butcher)
- Dungeon seeds are fixed to dlvl (dlvl 1 is seed 1, etc)
- Cheats: On/Off option in the game menu. Hold CTRL+SHIFT when clicking to toggle.
- Bunch of developer keyboard controls
- All normal quests are active

This unlocks additional controls:

- F4: Show FPS on control panel
- E: Show eflag on control panel
- P: Pause the game. Pressing again will frame advance. No way to unpause!

When cheats are on:

- Everything in the cheat patch
- The act of toggling gives all spells at level 0
- All quests are in the quest log, even "The Maze")txt")

PATCH_MAIN
{
    // This is the data segment, no need to adjust permissions
    *(uint32_t*)0x004BC158 = TRUE;
    return true;
}
