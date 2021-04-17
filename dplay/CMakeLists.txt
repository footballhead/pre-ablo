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
    fake_ordinals.def
    src/dllmain.cpp
    src/fake_ordinals.cpp
    src/functions.cpp
    src/patches.cpp
    src/util.cpp
    src/variables.cpp
    src/patches/always_load_flare.cpp
    src/patches/automap_fix.cpp
    src/patches/cheat.cpp
    src/patches/devmode.cpp
    src/patches/enable_fireman.cpp
    src/patches/fullgame.cpp
    src/patches/infinite_mana.cpp
    src/patches/infraring_fix.cpp
    src/patches/instacast.cpp
    src/patches/magic_find_x3.cpp
    src/patches/max_monster_types.cpp
    src/patches/mega_fix.cpp
    src/patches/monster_density_x2.cpp
    src/patches/music_nompq_fix.cpp
    src/patches/no_death_anim.cpp
    src/patches/no_tp_light.cpp
    src/patches/old_drlg.cpp
    src/patches/only_magma_demons.cpp
    src/patches/options_menu.cpp
    src/patches/panic_teleport.cpp
    src/patches/plrgfx_frame_fix.cpp
    src/patches/retail_music.cpp
    src/patches/randomize_leveltype.cpp
    src/patches/savegame_path_fix.cpp
    src/patches/skip_intros.cpp
    src/patches/skip_outro.cpp
    src/patches/snake_frame_fix.cpp
    src/patches/stone_curse_missile_fix.cpp
    src/patches/thunder_demon_missile_fix.cpp
    src/patches/too_much_hp_crash.cpp
    src/patches/towncast.cpp
    src/patches/tp_setlevel_fix.cpp
    src/patches/undead_crown.cpp
    src/patches/version_override.cpp
    src/patches/window.cpp
    src/patches/z_hook.cpp
    )
target_include_directories(dplay
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR} # Find version.h
        src
    )

# TODO should be "anyone that supports -s for stripping (gcc, clang (maybe))"
if (MINGW)
    set_target_properties(dplay
        PROPERTIES
            PREFIX "" # produce dplay.dll (not libdplay.dll)
            LINK_FLAGS_RELEASE "-s" # strip Release binaries to reduce size
            LINK_FLAGS_MINSIZEREL "-s" # ditto for MinSizeRel
        )
    target_link_libraries(dplay
        "-static-libstdc++"
        "-static-libgcc"
        )
endif()
