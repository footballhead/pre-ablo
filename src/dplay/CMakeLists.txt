cmake_minimum_required(VERSION 3.10)
project(dplaywrapper C CXX)

#
# Build dplay.dll
#

add_library(dplay SHARED
    dplay.cpp
    fake_ordinals.cpp
    fake_ordinals.def
    )
target_include_directories(dplay
    PRIVATE
        .. # Find our own headers
    )
target_link_libraries(dplay PRIVATE patches)

# produce dplay.dll (not libdplay.dll)
set_target_properties(dplay PROPERTIES PREFIX "")
