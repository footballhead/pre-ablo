cmake_minimum_required(VERSION 3.10)
project(reset_dungeon CXX)

# C++17 for optional
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(reset_dungeon WIN32
    io.cc
    main.cc
    saveload.cc
    )
