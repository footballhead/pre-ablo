#pragma once

#include <vector>

struct Patch {
    const char* name;
    bool (*main)();
    bool checked;
};

#define DECLARE_PATCH(id) bool id##_main()

DECLARE_PATCH(always_load_flare);
DECLARE_PATCH(automap_fix);
DECLARE_PATCH(cheat);
DECLARE_PATCH(devmode);
DECLARE_PATCH(fullgame);
DECLARE_PATCH(infraring_fix);
DECLARE_PATCH(max_monsters);
DECLARE_PATCH(mega_fix);
DECLARE_PATCH(music_nompq_fix);
DECLARE_PATCH(no_tp_light);
DECLARE_PATCH(old_drlg);
DECLARE_PATCH(options_menu);
DECLARE_PATCH(savegame_patch_fix);
DECLARE_PATCH(skip_intros);
DECLARE_PATCH(snake_frame_fix);
DECLARE_PATCH(stone_curse_missile_fix);
DECLARE_PATCH(thunder_demon_missile_fix);
DECLARE_PATCH(too_much_hp_crash);
DECLARE_PATCH(tp_setlevel_fix);
DECLARE_PATCH(undead_crown);
DECLARE_PATCH(version_override);
DECLARE_PATCH(window);
DECLARE_PATCH(always_load_flare);

std::vector<Patch>& get_patches();
bool save_checked_patches();
