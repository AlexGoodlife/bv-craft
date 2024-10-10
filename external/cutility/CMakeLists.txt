project(cutil)

add_library(cutil STATIC ./src/list.c ./src/queue.c)

target_include_directories(cutil PUBLIC ./include)
