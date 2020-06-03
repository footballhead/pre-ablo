cmake_minimum_required(VERSION 3.10)
project(joingroups CXX)

add_executable(joingroups src/main.cpp)
target_link_libraries(joingroups diabutil)
