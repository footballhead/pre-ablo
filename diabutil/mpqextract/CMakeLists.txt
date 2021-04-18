cmake_minimum_required(VERSION 3.10)
project(mpqextract CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(mpqextract src/main.cpp)
target_link_libraries(mpqextract
    storm_static
    )

install(TARGETS mpqextract RUNTIME DESTINATION bin)
