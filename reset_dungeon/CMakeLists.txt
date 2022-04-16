cmake_minimum_required(VERSION 3.10)
project(reset_dungeon C)

if (NOT WIN32)
    message(FATAL_ERROR "This program only compiles on 32-bit Windows!")
endif()

set(CMAKE_WIN32_EXECUTABLE TRUE)

# TODO force 32-bit build

#
# Configure version override
#

# TODO versioning for this tool?

#
# Build reset_dungeon.exe
#

add_executable(reset_dungeon main.c)

# TODO should be "anyone that supports -s for stripping (gcc, clang (maybe))"
# TODO should this go into the toolchain?
if (MINGW)
    set_target_properties(reset_dungeon
        PROPERTIES
            LINK_FLAGS_RELEASE "-s" # strip Release binaries to reduce size
            LINK_FLAGS_MINSIZEREL "-s" # ditto for MinSizeRel
        )
    target_link_libraries(reset_dungeon
        "-static-libgcc"
        )
endif()
