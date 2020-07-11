cmake_minimum_required(VERSION 3.10)
project(splitcel CXX)

add_executable(splitcel src/main.cpp)
target_link_libraries(splitcel diabutil)
