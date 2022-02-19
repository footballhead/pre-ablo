add_library(diabutil STATIC
    src/cel.cpp
    src/file.cpp
    src/palette.cpp
    )
target_include_directories(diabutil PUBLIC include)
target_link_libraries(diabutil stb_image_write)

# TODO: Write a macro for this
add_executable(test_palette tests/test_palette.cpp)
target_link_libraries(test_palette diabutil gtest gtest_main)
add_test(NAME test_palette COMMAND test_palette)