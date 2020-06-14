cmake_minimum_required(VERSION 3.10)
project(diabutil CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(stb_image_write)
add_subdirectory(diabutil)

add_subdirectory(cel2png)
add_subdirectory(drawtext)
add_subdirectory(joincel)
add_subdirectory(joingroups)
add_subdirectory(retail2prdemo)
add_subdirectory(splitcel)
add_subdirectory(splitgroups)
