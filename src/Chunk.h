#ifndef BV_CHUNK_H
#define BV_CHUNK_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Block.h"
#include "Shader.h"

#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16
#define CHUNK_HEIGHT 16


typedef struct{
    GLuint VAO, VBO;
    float vertices[(CHUNK_WIDTH*CHUNK_DEPTH*CHUNK_HEIGHT)*FLOATS_PER_CUBE];
    uint32_t faceCount; // Will change to face count later
}ChunkMesh;

typedef struct{
    ChunkMesh* mesh;
    uint32_t map[CHUNK_DEPTH][CHUNK_WIDTH*CHUNK_HEIGHT];
}Chunk;


ChunkMesh* chunckmesh_init();

// Chunk* chunk_build(uint32_t** map, uint32_t width,uint32_t height,uint32_t depth);
Chunk* chunk_build(uint32_t map[CHUNK_DEPTH][CHUNK_WIDTH*CHUNK_HEIGHT]);
void chunk_destroy(Chunk* chunk);
void chunk_prepare(Chunk* chunk);
void chunk_update(Chunk* chunk);
void chunk_render(Chunk* chunk);


#endif