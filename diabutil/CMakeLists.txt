cmake_minimum_required(VERSION 3.10)
project(diabutil CXX)

set(CMAKE_CXX_STANDARD 17)

#
# External libraries
#
add_subdirectory(dr_pcx)
add_subdirectory(stb_image_write)
include(stormlib.cmake)

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
add_subdirectory(mpqadd)
add_subdirectory(mpqextract)
add_subdirectory(retail2prdemo)
add_subdirectory(splitcel)
add_subdirectory(splitgroups)
