# 3.8+ is required for CMAKE_CXX_STANDARD 17
cmake_minimum_required(VERSION 3.8)
project(diabutil CXX)

set(CMAKE_CXX_STANDARD 17)

include(stormlib.cmake)
add_subdirectory(mpqadd)
add_subdirectory(mpqextract)
