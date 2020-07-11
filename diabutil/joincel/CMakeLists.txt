cmake_minimum_required(VERSION 3.10)
project(joincel CXX)

add_executable(joincel src/main.cpp)
target_link_libraries(joincel diabutil)
