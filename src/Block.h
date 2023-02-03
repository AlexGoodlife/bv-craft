#ifndef BV_BLOCK_H
#define BV_BLOCK_H

#include "math/vec.h"
#include "stdint.h"
#include "string.h" // memcpy
#include "stdio.h"

#define N_FACES 6
#define VERTEXES_PER_FACE 6
#define FACE_INDICES_COUNT 6

#define ATLAS_WIDTH 256
#define ATLAS_HEIGHT 256
#define TEXTURE_WIDTH 16
#define TEXTURE_HEIGHT 16



enum FaceOrder{
    FaceOrder_Front,
    FaceOrder_Back,
    FaceOrder_Top,
    FaceOrder_Bottom,
    FaceOrder_Left,
    FaceOrder_Right,
    FaceOrder_End
};

enum BlockID{
    Air,
    Gravel,
    Dirt,
};

typedef struct{
    vec3_s vertexData[VERTEXES_PER_FACE];
    vec2_s uvData[VERTEXES_PER_FACE];
    uint32_t indicesData[FACE_INDICES_COUNT];
    uint32_t texture_pos_offset;
}FaceMesh;


typedef struct
{
    FaceMesh faces[N_FACES];
    uint32_t vertexCount;
    uint32_t indicesCount;
    uint32_t id;
}BlockMesh;

BlockMesh blockmesh_build(enum BlockID ID);

void blockmesh_copyVertexData(BlockMesh *mesh, float* vertices);

void blockmesh_copyIndicesData(BlockMesh* mesh, uint32_t* indices);
#endif