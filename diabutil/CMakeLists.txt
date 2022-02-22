cmake_minimum_required(VERSION 3.10)
project(diabutil CXX)

set(CMAKE_CXX_STANDARD 17)
include(CTest)

option(WITH_STORMLIB "Compile StormLib and programs that use it" ON)

#
# External libraries
#
add_subdirectory(dr_pcx)
add_subdirectory(stb_image_write)
add_subdirectory(stb_image)
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
add_subdirectory(fixamp)
add_subdirectory(joincel)
add_subdirectory(joingroups)
add_subdirectory(png2cel)
add_subdirectory(retail2prdemo)
add_subdirectory(splitcel)
add_subdirectory(splitgroups)

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
