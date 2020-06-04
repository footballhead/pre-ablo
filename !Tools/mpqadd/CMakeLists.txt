cmake_minimum_required(VERSION 3.10)
project(mpqadd CXX)

# TODO: Windows support
find_library(STORM_LIBRARY libstorm.so PATHS ENV STORM_ROOT PATH_SUFFIXES lib)
find_path(STORM_INCLUDE_DIR StormLib.h PATHS ENV STORM_ROOT PATH_SUFFIXES include)

set(CMAKE_CXX_STANDARD 17)

add_executable(mpqadd src/main.cpp)
target_link_libraries(mpqadd
    ${STORM_LIBRARY}
    )
target_include_directories(mpqadd 
    PRIVATE 
        ${STORM_INCLUDE_DIR}
    )
