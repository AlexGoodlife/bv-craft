#ifndef BV_BLOCK_H
#define BV_BLOCK_H

#include "math/vec.h"
#include "stdint.h"
#include "string.h" // memcpy
#include "stdlib.h" // memcpy

#define N_FACES 6
#define VERTEXES_PER_FACE 6
#define FACE_INDICES_COUNT 6
#define FLOATS_PER_VERTEX 5

#define TRIANGLES_PER_FACE 2

#define FLOATS_PER_CUBE VERTEXES_PER_FACE*N_FACES*FLOATS_PER_VERTEX

#define ATLAS_WIDTH 512
#define ATLAS_HEIGHT 256
#define TEXTURE_WIDTH 16
#define TEXTURE_HEIGHT 16

enum FaceOrder{
    FaceOrder_Miss = -1,
    FaceOrder_Front,
    FaceOrder_Back,
    FaceOrder_Top,
    FaceOrder_Bottom,
    FaceOrder_Left,
    FaceOrder_Right,
    FaceOrder_End
};

enum BlockID{
    Gravel,
    Snow,
    Grass,
    Water,
    Dirt,
    Stone,
    Block_count
};

typedef struct{
    vec3_s vertexData[VERTEXES_PER_FACE];
    vec2_s uvData[VERTEXES_PER_FACE];
    int texture_pos_offset;
}FaceMesh;


typedef struct
{
    FaceMesh faces[N_FACES];
    uint32_t vertexCount;
    uint32_t indicesCount;
    enum BlockID id;
    int texture_atlas_position;
}BlockMesh;


extern BlockMesh* all_blocks;

BlockMesh blockmesh_build(enum BlockID ID);

// void blockmesh_copyVertexData(BlockMesh *mesh, float* vertices);
void blockmesh_copyVertexData(BlockMesh *mesh, float* vertices, vec3_s positionOffset);

void blockmesh_copyIndicesData(BlockMesh* mesh, uint32_t* indices);

void blockmesh_buildAllBlocks();
void facemesh_copyVertexData(FaceMesh* mesh,float* vertices, vec3_s positionOffset);
enum FaceOrder block_intersect(vec3_s block_pos, vec3_s ray_origin,vec3_s ray_end);
#endif
