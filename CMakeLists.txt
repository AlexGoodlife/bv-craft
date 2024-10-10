cmake_minimum_required(VERSION 3.10)
project(bvcraft VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(bvcraft main.c)
add_subdirectory(external)
add_subdirectory(src)
target_link_libraries(bvcraft cutil bv glfw glad)

add_custom_command(
         TARGET bvcraft POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory              
             ${CMAKE_SOURCE_DIR}/src/shaders $<TARGET_FILE_DIR:bvcraft>/shaders
         COMMENT "Copying shaders" VERBATIM
         )
add_custom_command(
         TARGET bvcraft POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory              
             ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:bvcraft>/resources
         COMMENT "Copying resources" VERBATIM
         )
