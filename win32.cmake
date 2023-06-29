# These are things that must be build for 32 bit Windows.
# On Windows, uses MSVC x86 toolchain. On Linux, use mingw

add_subdirectory(third_party/glew-2.2.0/build/cmake)
add_subdirectory(src/ddraw)
add_subdirectory(src/dplay)
add_subdirectory(src/patches)
add_subdirectory(src/reset_dungeon)