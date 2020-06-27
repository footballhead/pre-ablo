#include "patches.hpp"

#include <algorithm>
#include <fstream>
#include <string>

// At global scope so the extern can resolve.
// While the extern isn't really safe, it prevents the entire code base from recompiling with every new patch.
#define DECLARE_PATCH(id) extern bool id##_main(); extern char const* id##_description;
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
DECLARE_PATCH(skip_outro);
DECLARE_PATCH(snake_frame_fix);
DECLARE_PATCH(stone_curse_missile_fix);
DECLARE_PATCH(thunder_demon_missile_fix);
DECLARE_PATCH(too_much_hp_crash);
DECLARE_PATCH(tp_setlevel_fix);
DECLARE_PATCH(undead_crown);
DECLARE_PATCH(version_override);
DECLARE_PATCH(window);

namespace {

#define REGISTER_PATCH(id) {#id, id##_description, id##_main, false}
#define REGISTER_PATCH_RECOMMENDED(id) {#id, id##_description, id##_main, true}

const Patch patches_registry[] = {
    REGISTER_PATCH_RECOMMENDED(always_load_flare),
    REGISTER_PATCH_RECOMMENDED(automap_fix),
    REGISTER_PATCH(cheat),
    REGISTER_PATCH(devmode),
    REGISTER_PATCH_RECOMMENDED(fullgame),
    REGISTER_PATCH_RECOMMENDED(infraring_fix),
    REGISTER_PATCH(max_monsters),
    REGISTER_PATCH_RECOMMENDED(mega_fix),
#ifndef NDEBUG
    REGISTER_PATCH(music_nompq_fix),
#endif
    REGISTER_PATCH_RECOMMENDED(no_tp_light),
    REGISTER_PATCH(old_drlg),
    REGISTER_PATCH_RECOMMENDED(options_menu),
#ifndef NDEBUG
    REGISTER_PATCH(savegame_patch_fix),
#endif
    REGISTER_PATCH_RECOMMENDED(skip_intros),
#ifndef NDEBUG
    REGISTER_PATCH(skip_outro),
#endif
    REGISTER_PATCH_RECOMMENDED(snake_frame_fix),
    REGISTER_PATCH_RECOMMENDED(stone_curse_missile_fix),
    REGISTER_PATCH_RECOMMENDED(thunder_demon_missile_fix),
    REGISTER_PATCH_RECOMMENDED(too_much_hp_crash),
    REGISTER_PATCH_RECOMMENDED(tp_setlevel_fix),
    REGISTER_PATCH_RECOMMENDED(undead_crown),
    REGISTER_PATCH_RECOMMENDED(version_override),
#ifndef NDEBUG
    REGISTER_PATCH(window),
#endif
};

#define NUM_PATCHES (sizeof(patches_registry) / sizeof(Patch))

constexpr auto patch_filename = "last_patch.txt";
auto loaded_patches = false;

} // namespace

std::vector<Patch>& get_patches()
{
    static std::vector<Patch> patches;

    if (loaded_patches) {
        return patches;
    }

    patches = std::vector<Patch>(patches_registry, patches_registry + NUM_PATCHES);
    loaded_patches = true;

    // If can't load file (for one reason or another) then use pre-compiled recommendations.
    std::ifstream in{patch_filename};
    if (!in.is_open()) {
        printf("No last_patch.txt, initializing defaults\n");
        return patches;
    }

    // Otherwise, re-init checked state for each patch based on saved config (last_patch.txt)
    for (auto& patch : patches) {
        patch.checked = false;
    }

    std::string line;
    while (std::getline(in, line)) {
        auto I = std::find_if(begin(patches), end(patches), [&line](Patch const& patch) { return patch.name == line; });
        if (I == end(patches)) {
            printf("Ignoring undefined patch: %s\n", line.c_str());
            continue;
        }

        I->checked = true;
    }

    return patches;
}

bool save_checked_patches()
{
    std::ofstream out{patch_filename};
    if (!out.is_open()) {
        printf("Could not open last_patch.txt to save\n");
        return false;
    }

    for (auto const& patch : get_patches()) {
        if (patch.checked) {
            out << patch.name << '\n';
        }
    }
    return true;
}
