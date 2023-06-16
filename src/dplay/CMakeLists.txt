cmake_minimum_required(VERSION 3.10)
project(dplaywrapper C CXX)

#
# Configure version override
#

# Override with cmake -DPRDE_VERSION=foo
set(PRDE_VERSION "IN-DEV" CACHE STRING "Version shown on main menu")
message(STATUS "PRDE_VERSION is: ${PRDE_VERSION}")
configure_file(version.h.in version.h @ONLY)

#
# Build dplay.dll
#

add_library(dplay SHARED
    dplay.cpp
    fake_ordinals.cpp
    fake_ordinals.def
    functions.cpp
    patches.cpp
    util.cpp
    variables.cpp
    patches/always_load_flare.cpp
    patches/automap_fix.cpp
    patches/cheat.cpp
    patches/devmode.cpp
    patches/enable_cut_monsters.cpp
    patches/enable_unfinished_monsters.cpp
    patches/fullgame.cpp
    patches/infinite_mana.cpp
    patches/infraring_fix.cpp
    patches/instacast.cpp
    patches/load_mpq_customizations.cpp
    patches/magic_find_x3.cpp
    patches/max_monster_types.cpp
    patches/mega_fix.cpp
    patches/monster_density_x2.cpp
    patches/music_nompq_fix.cpp
    patches/no_death_anim.cpp
    patches/no_tp_light.cpp
    patches/old_drlg.cpp
    patches/only_magma_demons.cpp
    patches/options_menu.cpp
    patches/panic_teleport.cpp
    patches/plrgfx_frame_fix.cpp
    patches/retail_music.cpp
    patches/randomize_leveltype.cpp
    patches/savegame_path_fix.cpp
    patches/skip_intros.cpp
    patches/skip_outro.cpp
    patches/snake_frame_fix.cpp
    patches/stone_curse_missile_fix.cpp
    patches/thunder_demon_missile_fix.cpp
    patches/too_much_hp_crash.cpp
    patches/towncast.cpp
    patches/tp_setlevel_fix.cpp
    patches/undead_crown.cpp
    patches/version_override.cpp
    patches/window.cpp
    patches/z_hook.cpp
    )
target_include_directories(dplay
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR} # Find version.h
        . # Help patches find our headers
    )

# produce dplay.dll (not libdplay.dll)
set_target_properties(dplay PROPERTIES PREFIX "")
