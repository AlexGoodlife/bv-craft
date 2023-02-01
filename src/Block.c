#include "Block.h"

static vec3_s ForwardFace[]  =  
{
        vec3(-0.5f, -0.5f, -0.5f),
        vec3(-0.5f, 0.5f, -0.5f),
        vec3(0.5f, 0.5f, -0.5f),
        vec3(0.5f, -0.5f, -0.5f)
};

static uint32_t ForwardTris[]  =  
{
    0,2,1,0,3,2
};


static vec3_s BackFace[]  =  
{
        vec3(-.5f, -.5f, 0.5f),
        vec3(-.5f, .5f, 0.5f),
        vec3(.5f, .5f, 0.5f),
        vec3(.5f, -.5f, 0.5f)
};

static uint32_t BackTris[]  =  
{
    4,5,6,4,6,7
};

static vec3_s UpFace[]  =  
{
        vec3(-.5f, .5f, -0.5f),
        vec3(-.5f, .5f, .5f),
        vec3(.5f, .5f, .5f),
        vec3(.5f, .5f, -.5f)
};

static uint32_t UpTris[]  =  
{
    8,9,10,8,10,11
};

static vec3_s DownFace[]  =  
{
        vec3(-.5f, -.5f, -.5f),
        vec3(-.5f, -.5f, .5f),
        vec3(.5f, -.5f, .5f),
        vec3(.5f, -.5f, -.5f)
};

static uint32_t DownTris[]  =  
{
    12,14,13,12,15,14
};

static vec3_s LeftFace[]  = 
{
        vec3(-.5f, -.5f, -.5f),
        vec3(-.5f, -.5f, .5f),
        vec3(-.5f, .5f, .5f),
        vec3(-.5f, .5f, -.5f)
};

static uint32_t LeftTris[]  =  
{
    16,17,18,16,18,19
};

static vec3_s RightFace[]  =
{
    vec3(0.5f, -0.5f, -0.5f),
    vec3(0.5f, -0.5f, 0.5f),
    vec3(0.5f, 0.5f, 0.5f),
    vec3(0.5f, 0.5f, -0.5f)
};

static uint32_t RightTris[] = 
{
    20,22,21,20,23,22
};



FaceMesh facemesh_build(vec3_s* vertexData, uint32_t* indiceData){
    FaceMesh result;
    memcpy(result.vertexData, vertexData, sizeof(vec3_s) * VERTEXES_PER_FACE);
    memcpy(result.indicesData, indiceData, sizeof(uint32_t) * FACE_INDICES_COUNT);
    return result;
}


BlockMesh blockmesh_build(){
    BlockMesh result;
    result.faces[FaceOrder_Front] = facemesh_build(ForwardFace, ForwardTris);
    result.faces[FaceOrder_Back] = facemesh_build(BackFace, BackTris);
    result.faces[FaceOrder_Top] = facemesh_build(UpFace, UpTris);
    result.faces[FaceOrder_Bottom] = facemesh_build(DownFace, DownTris);
    result.faces[FaceOrder_Left] = facemesh_build(LeftFace, LeftTris);
    result.faces[FaceOrder_Right] = facemesh_build(RightFace, RightTris);
    return result;
}
