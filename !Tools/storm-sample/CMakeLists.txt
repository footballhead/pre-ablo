cmake_minimum_required(VERSION 3.10)

project(storm_test CXX)

add_executable(storm_test main.cpp)
target_link_libraries(storm_test shlwapi)