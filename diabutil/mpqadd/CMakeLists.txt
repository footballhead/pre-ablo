project(mpqadd CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(mpqadd src/main.cpp)
target_link_libraries(mpqadd
    storm_static
    )

install(TARGETS mpqadd RUNTIME DESTINATION bin)
