cmake_minimum_required(VERSION 3.10)
project(preablo C CXX)

set(PREABLO_TARGET "host" CACHE STRING "Which code to compile. Either host (default) or win32")

if (PREABLO_TARGET STREQUAL "host")
    include(host.cmake)
elseif (PREABLO_TARGET STREQUAL "win32")
    include(win32.cmake)
else()
    message(FATAL_ERROR "Unknown PREABLO_TARGET")
endif()