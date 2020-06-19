cmake_minimum_required(VERSION 3.10)

project(dplaywrapper C CXX)

add_library(dplay SHARED dll.cpp fake_ordinals.def fake_ordinals.cpp)

# produce `dplay.dll` (not libdplay.dll)
set_target_properties(dplay PROPERTIES PREFIX "")
