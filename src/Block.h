#ifndef BV_BLOCK_H
#define BV_BLOCK_H

#include "math/vec.h"
#include "stdint.h"
#include "string.h" // memcpy
#include "stdio.h"

#define N_FACES 6
#define VERTEXES_PER_FACE 4
#define FACE_INDICES_COUNT 6

enum FaceOrder{
    FaceOrder_Front,
    FaceOrder_Back,
    FaceOrder_Top,
    FaceOrder_Bottom,
    FaceOrder_Left,
    FaceOrder_Right,
    FaceOrder_End
};

typedef struct{
    vec3_s vertexData[VERTEXES_PER_FACE];
    // vec2_s uvData[VERTEXES_PER_FACE];
    uint32_t indicesData[FACE_INDICES_COUNT];
}FaceMesh;


typedef struct
{
    FaceMesh faces[N_FACES];
}BlockMesh;

BlockMesh blockmesh_build();


#endif