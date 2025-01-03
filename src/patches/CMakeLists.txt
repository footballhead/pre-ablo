#
# Configure version override
#

# Override with cmake -DPRDE_VERSION=foo
set(PRDE_VERSION "IN-DEV" CACHE STRING "Version shown on main menu")
message(STATUS "PRDE_VERSION is: ${PRDE_VERSION}")
configure_file(version.h.in version.h @ONLY)

#
# Build patches lib
#

add_library(patches STATIC
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
    patches/infraring_fix.cpp
    patches/load_mpq_customizations.cpp
    patches/mega_fix.cpp
    patches/music_nompq_fix.cpp
    patches/no_tp_light.cpp
    patches/old_drlg.cpp
    patches/options_menu.cpp
    patches/panic_teleport.cpp
    patches/plrgfx_frame_fix.cpp
    patches/retail_music.cpp
    patches/savegame_path_fix.cpp
    patches/skip_intros.cpp
    patches/stone_curse_missile_fix.cpp
    patches/thunder_demon_missile_fix.cpp
    patches/too_much_hp_crash.cpp
    patches/tp_setlevel_fix.cpp
    patches/undead_crown.cpp
    patches/version_override.cpp
    patches/window.cpp
    patches/z_hook.cpp
    )
target_include_directories(patches
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR} # Find version.h
        . # Help patches/ find our headers
    )

