#include "patches.hpp"

#include <algorithm>
#include <fstream>
#include <string>

// At global scope so the extern can resolve.
// While the extern isn't really safe, it prevents the entire code base from recompiling with every new patch.
// (The alternate approach is to declare in a header which all patches include, which will cause a recompile of everything on a single modification)
#define DECLARE_PATCH(id) extern bool id##_main(); extern char const* id##_description;

// Order alphabetically!
DECLARE_PATCH(always_load_flare);
DECLARE_PATCH(automap_fix);
DECLARE_PATCH(cheat);
DECLARE_PATCH(devmode);
DECLARE_PATCH(enable_cut_monsters);
DECLARE_PATCH(enable_unfinished_monsters);
DECLARE_PATCH(fullgame);
DECLARE_PATCH(infinite_mana);
DECLARE_PATCH(infraring_fix);
DECLARE_PATCH(instacast);
DECLARE_PATCH(load_mpq_customizations);
DECLARE_PATCH(magic_find_x3);
DECLARE_PATCH(max_monster_types);
DECLARE_PATCH(mega_fix);
DECLARE_PATCH(monster_density_x2);
DECLARE_PATCH(music_nompq_fix);
DECLARE_PATCH(no_death_anim);
DECLARE_PATCH(no_tp_light);
DECLARE_PATCH(old_drlg);
DECLARE_PATCH(only_magma_demons);
DECLARE_PATCH(options_menu);
DECLARE_PATCH(panic_teleport);
DECLARE_PATCH(plrgfx_frame_fix);
DECLARE_PATCH(randomize_leveltype);
DECLARE_PATCH(retail_music);
DECLARE_PATCH(savegame_patch_fix);
DECLARE_PATCH(skip_intros);
DECLARE_PATCH(skip_outro);
DECLARE_PATCH(snake_frame_fix);
DECLARE_PATCH(stone_curse_missile_fix);
DECLARE_PATCH(thunder_demon_missile_fix);
DECLARE_PATCH(too_much_hp_crash);
DECLARE_PATCH(towncast);
DECLARE_PATCH(tp_setlevel_fix);
DECLARE_PATCH(undead_crown);
DECLARE_PATCH(version_override);
DECLARE_PATCH(window);
DECLARE_PATCH(z_hook);

namespace {

#define REGISTER_PATCH(id) {#id, id##_description, id##_main, false, nullptr},
#define REGISTER_PATCH_RECOMMENDED(id) {#id, id##_description, id##_main, true, nullptr},

#ifndef NDEBUG
#define REGISTER_PATCH_DEBUG(id) REGISTER_PATCH(id)
#else
#define REGISTER_PATCH_DEBUG(id)
#endif

// If a patch is not registered here then it does not exist, i.e. it can't be toggled. Probably shouldn't even be compiled...
// To be registered, the patch must first be declared above (at global scope).
// Order alphabetically!
const Patch patches_registry[] = {
    REGISTER_PATCH_RECOMMENDED(always_load_flare)
    REGISTER_PATCH_RECOMMENDED(automap_fix)
    REGISTER_PATCH(cheat)
    REGISTER_PATCH(devmode)
    REGISTER_PATCH_RECOMMENDED(enable_cut_monsters)
    REGISTER_PATCH_DEBUG(enable_unfinished_monsters)
    REGISTER_PATCH_RECOMMENDED(fullgame)
    REGISTER_PATCH_DEBUG(infinite_mana)
    REGISTER_PATCH_RECOMMENDED(infraring_fix)
    REGISTER_PATCH_DEBUG(instacast)
    {"load_mpq_customizations", load_mpq_customizations_description, load_mpq_customizations_main, /*checked=*/true, "pre-ablo.mpq"},
    REGISTER_PATCH_DEBUG(magic_find_x3)
    REGISTER_PATCH_DEBUG(max_monster_types)
    REGISTER_PATCH_DEBUG(mega_fix)
    REGISTER_PATCH_DEBUG(monster_density_x2)
    REGISTER_PATCH_DEBUG(music_nompq_fix)
    REGISTER_PATCH_DEBUG(no_death_anim)
    REGISTER_PATCH_RECOMMENDED(no_tp_light)
    REGISTER_PATCH(old_drlg)
    REGISTER_PATCH_DEBUG(only_magma_demons)
    REGISTER_PATCH_RECOMMENDED(options_menu)
    REGISTER_PATCH_DEBUG(panic_teleport)
    REGISTER_PATCH_RECOMMENDED(plrgfx_frame_fix)
    REGISTER_PATCH_DEBUG(randomize_leveltype)
    REGISTER_PATCH_DEBUG(retail_music)
    REGISTER_PATCH_DEBUG(savegame_patch_fix)
    REGISTER_PATCH_RECOMMENDED(skip_intros)
    REGISTER_PATCH_DEBUG(skip_outro)
    REGISTER_PATCH_DEBUG(snake_frame_fix)
    REGISTER_PATCH_RECOMMENDED(stone_curse_missile_fix)
    REGISTER_PATCH_RECOMMENDED(thunder_demon_missile_fix)
    REGISTER_PATCH_RECOMMENDED(too_much_hp_crash)
    REGISTER_PATCH_DEBUG(towncast)
    REGISTER_PATCH_RECOMMENDED(tp_setlevel_fix)
    REGISTER_PATCH_RECOMMENDED(undead_crown)
    REGISTER_PATCH_RECOMMENDED(version_override)
    REGISTER_PATCH_DEBUG(window)
    REGISTER_PATCH_DEBUG(z_hook)
};

#define NUM_PATCHES (sizeof(patches_registry) / sizeof(Patch))

constexpr auto patch_filename = "last_patch.txt";
auto loaded_patches = false;

} // namespace

std::vector<Patch>& get_patches()
{
    static std::vector<Patch> patches;

    // Lazy load patches on first call and store in static. Subsequent calls return the cached load.
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
