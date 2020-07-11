project(stb_image_write C)
add_library(stb_image_write STATIC src/stb_image_write.c)
target_include_directories(stb_image_write PUBLIC include)
