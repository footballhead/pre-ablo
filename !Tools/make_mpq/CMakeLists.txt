cmake_minimum_required(VERSION 3.10)
project(make_mpq CXX)

# TODO: Windows support
find_library(STORM_LIBRARY libstorm.so PATHS ENV STORM_ROOT PATH_SUFFIXES lib)
find_path(STORM_INCLUDE_DIR StormLib.h PATHS ENV STORM_ROOT PATH_SUFFIXES include)

set(CMAKE_CXX_STANDARD 17)

add_executable(make_mpq src/main.cpp)
target_link_libraries(make_mpq
    ${STORM_LIBRARY}
    )
target_include_directories(make_mpq 
    PRIVATE 
        ${STORM_INCLUDE_DIR}
    )
