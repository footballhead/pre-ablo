cmake_minimum_required(VERSION 3.10)
project(fixamp CXX)

add_executable(fixamp src/main.cpp)
target_link_libraries(fixamp diabutil)
