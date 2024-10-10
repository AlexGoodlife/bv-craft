cmake_minimum_required(VERSION 3.10)
project(bvcraft VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(bvcraft main.c)
add_subdirectory(external)
add_subdirectory(src)
target_link_libraries(bvcraft cutil bv glfw glad)
