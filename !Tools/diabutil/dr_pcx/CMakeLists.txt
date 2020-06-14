project(dr_pcx C)
add_library(dr_pcx STATIC src/dr_pcx.c)
target_include_directories(dr_pcx PUBLIC include)
