# These are all the tools that run on the host OS

# NOTE: StormLib won't compile on Apple
include(stormlib.cmake)
add_subdirectory(src/mpqadd)
add_subdirectory(src/mpqextract)