add_library(diabutil STATIC
    src/cel.cpp
    src/file.cpp
    src/palette.cpp
    )
target_include_directories(diabutil PUBLIC include)
