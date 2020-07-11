cmake_minimum_required(VERSION 3.10)
project(diabutil CXX)

set(CMAKE_CXX_STANDARD 17)

#
# External libraries
#
add_subdirectory(dr_pcx)
add_subdirectory(stb_image_write)

# StormLib wasn't designed to compile with cmake on Windows... so we have to hack around it
# Compile static also because there's no DLL-with-exports config
set(WITH_STATIC ON)
add_subdirectory(StormLib)
# Prefer that clients use our hack header to avoid some linker problems
target_include_directories(storm_static
    BEFORE INTERFACE
        StormLib_hack
    )
# Clients should only use storm_static target, not storm target. At least on Windows

#
# Internal library
#
add_subdirectory(diabutil)

#
# Internal command-lines
#
add_subdirectory(cel2png)
add_subdirectory(drawtext)
add_subdirectory(drawtextpcx)
add_subdirectory(joincel)
add_subdirectory(joingroups)
add_subdirectory(mpqadd)
add_subdirectory(retail2prdemo)
add_subdirectory(splitcel)
add_subdirectory(splitgroups)
