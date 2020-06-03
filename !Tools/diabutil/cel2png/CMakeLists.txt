add_executable(cel2png src/main.cpp)
target_link_libraries(cel2png
    diabutil
    stb_image_write
    )
