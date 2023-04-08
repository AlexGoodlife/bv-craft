#ifndef BV_CHUNK_H
#define BV_CHUNK_H

#include "../glad/glad.h"
#include <GLFW/glfw3.h>

#include "Block.h"
#include "Shader.h"

#define CHUNK_WIDTH 32
#define CHUNK_DEPTH 32
#define CHUNK_HEIGHT 32

#define INDEXCHUNK(x,y,z) INDEX3D(x,y,z,CHUNK_WIDTH,CHUNK_HEIGHT)

typedef struct {
  GLuint VAO, VBO;
  float *vertices;
  uint32_t faceCount;
} ChunkMesh;

typedef struct {
  ChunkMesh *mesh;
  uint32_t *map; // This is 3D
  bool is_updated;
  bool is_prepared;
} Chunk;

ChunkMesh *chunckmesh_init();

// Chunk* chunk_build(uint32_t** map, uint32_t width,uint32_t height,uint32_t
// depth);
Chunk *chunk_build(uint32_t map[CHUNK_DEPTH * CHUNK_WIDTH * CHUNK_HEIGHT]);
void chunk_destroy(Chunk **chunk);
void chunk_prepare(Chunk *chunk);
// void chunk_update(Chunk* chunk);
void chunk_update(Chunk **map, uint32_t map_width, uint32_t map_height,
                  uint32_t chunk_pos, Chunk *chunk);
void chunk_render(Chunk *chunk);

#endif
