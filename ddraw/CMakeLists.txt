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
target_link_libraries(ddraw OpenGL::GL)

# TODO should be "anyone that supports -s for stripping (gcc, clang (maybe))"
if (MINGW)
    set_target_properties(ddraw
        PROPERTIES
            PREFIX "" # produce ddraw.dll (not ddraw.dll)
            LINK_FLAGS_RELEASE "-s" # strip Release binaries to reduce size
            LINK_FLAGS_MINSIZEREL "-s" # ditto for MinSizeRel
        )
    target_link_libraries(ddraw
        "-static-libstdc++"
        "-static-libgcc"
        )
endif()
