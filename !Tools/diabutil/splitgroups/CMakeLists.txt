cmake_minimum_required(VERSION 3.10)
project(splitgroups CXX)

add_executable(splitgroups src/main.cpp)
target_link_libraries(splitgroups diabutil)
