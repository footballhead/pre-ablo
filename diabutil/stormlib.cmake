# StormLib wasn't designed to compile with cmake on Windows... so we have to hack around it
# Compile static also because there's no DLL-with-exports config
SET(WITH_STATIC ON CACHE BOOL "Compile static linked librar (StormLib)")
add_subdirectory(StormLib)
# Prefer that clients use our hack header to avoid some linker problems
target_include_directories(storm_static
    BEFORE INTERFACE
        StormLib_hack
    )
# Clients should only use storm_static target, not storm target. At least on Windows