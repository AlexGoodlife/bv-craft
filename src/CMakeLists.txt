add_library(bv
  World.c
  thread.c
  worldgen.c
  Chunk.c
  Block.c
  Shader.c
  state.c
  Camera.c
)

target_include_directories(bv PUBLIC .)
target_include_directories(bv PUBLIC math)
target_include_directories(bv PUBLIC ../external/FastNoiseLite)
target_include_directories(bv PUBLIC ../external/stb_image)
target_include_directories(bv PRIVATE shaders)
target_link_libraries(bv PRIVATE glfw glad cutil)
