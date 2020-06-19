cmake_minimum_required(VERSION 3.10)
project(dplaywrapper C CXX)

#
# Configure version override
#

# Override with cmake -DPRDE_VERSION=foo
set(PRDE_VERSION "IN-DEV" CACHE STRING "Version shown on main menu")
message(STATUS "PRDE_VERSION is: ${PRDE_VERSION}")
configure_file(version.h.in version.h @ONLY)

#
# Build dplay.dll
#

add_library(dplay SHARED
    dll.cpp
    fake_ordinals.def
    fake_ordinals.cpp
    )
target_include_directories(dplay
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR} # Find version.h
    )
set_target_properties(dplay
    PROPERTIES
        PREFIX "" # produce dplay.dll (not libdplay.dll)
        LINK_FLAGS_RELEASE "-s" # strip Release binaries to reduce size
        LINK_FLAGS_MINSIZEREL "-s" # ditto for MinSizeRel
        )
