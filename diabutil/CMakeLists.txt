cmake_minimum_required(VERSION 3.10)
project(diabutil CXX)

option(WITH_STORMLIB "Compile StormLib and programs that use it" ON)

set(CMAKE_CXX_STANDARD 17)
include(CTest)

#
# External libraries
#
add_subdirectory(dr_pcx)
add_subdirectory(stb_image_write)
add_subdirectory(stb_image)

if (MSVC)
    # Quick linker fix for googletest on Windows. I don't actually know what the
    # repurcussions of this are outside of allowing me to compile
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
add_subdirectory(googletest)

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
add_subdirectory(png2cel)
if (MSVC)
    add_subdirectory(allitemslist)
endif()

#
# StormLib is causing problems so isolate
#

# TODO: StormLib won't compile on Apple Clang 13
if(WITH_STORMLIB)
    include(stormlib.cmake)
    add_subdirectory(mpqadd)
    add_subdirectory(mpqextract)
endif()

# TODO: Run unit + integration tests in CI
