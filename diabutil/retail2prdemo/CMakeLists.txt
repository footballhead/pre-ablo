cmake_minimum_required(VERSION 3.10)
project(retail2prdemo CXX)

add_executable(retail2prdemo src/main.cpp)
target_link_libraries(retail2prdemo diabutil)
