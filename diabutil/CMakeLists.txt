# 3.8+ is required for CMAKE_CXX_STANDARD 17
cmake_minimum_required(VERSION 3.8)
project(diabutil CXX)

option(WITH_STORMLIB "Compile StormLib and programs that use it" ON)

set(CMAKE_CXX_STANDARD 17)

#
# StormLib is causing problems so isolate
#

# TODO: StormLib won't compile on Apple Clang 13
if(WITH_STORMLIB)
    include(stormlib.cmake)
    add_subdirectory(mpqadd)
    add_subdirectory(mpqextract)
endif()
