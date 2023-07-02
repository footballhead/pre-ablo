# These are things that must be build for 32 bit Windows.
# On Windows, uses MSVC x86 toolchain. On Linux, use mingw

include(glew.cmake)
add_subdirectory(src/ddraw)
add_subdirectory(src/dplay)
add_subdirectory(src/patches)
add_subdirectory(src/reset_dungeon)