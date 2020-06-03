cmake_minimum_required(VERSION 3.10)
project(mpq_extract_all CXX)

# TODO: Windows support
find_library(STORM_LIBRARY libstorm.so PATHS ENV STORM_ROOT PATH_SUFFIXES lib)
find_path(STORM_INCLUDE_DIR StormLib.h PATHS ENV STORM_ROOT PATH_SUFFIXES include)

set(CMAKE_CXX_STANDARD 17)

add_executable(mpq_extract_all src/main.cpp)
target_link_libraries(mpq_extract_all
    ${STORM_LIBRARY}
    stdc++fs
    )
target_include_directories(mpq_extract_all 
    PRIVATE 
        ${STORM_INCLUDE_DIR}
    )
