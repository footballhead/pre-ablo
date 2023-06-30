cmake_minimum_required(VERSION 3.10)
project(ddraw C CXX)

# C++20 for designated initializers. C++17 for optional
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

add_library(ddraw SHARED
    ddraw.def
    ddraw.cpp
    ddraw_impl.cpp
    palette.cpp
    surface.cpp
    )
target_link_libraries(ddraw OpenGL::GL glew_s)

# produce ddraw.dll (not libddraw.dll)
set_target_properties(ddraw PROPERTIES PREFIX "")