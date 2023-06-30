# The GLEW CMakeLists was designed to be used with install targets only. Add hacks to allow using build-tree targets
add_subdirectory(third_party/glew-2.2.0/build/cmake)
target_include_directories(glew_s PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew-2.2.0/include>)