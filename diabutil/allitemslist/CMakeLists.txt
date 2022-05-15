# TODO: only works on win32
add_executable(allitemslist main.cpp)
target_link_libraries(allitemslist diabutil)
# For #include <decomp/*.h>
target_include_directories(allitemslist PRIVATE ../..)