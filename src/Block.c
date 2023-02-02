#include "Block.h"

float blockmesh_mapUVx(float u, uint32_t blockID,uint32_t textureWidth, uint32_t textureHeight, uint32_t atlasWidth, uint32_t atlasHeight){
    return ((uint32_t)((textureWidth- 1)*(blockID + u)) % (atlasWidth-1))/textureWidth;
}
float blockmesh_mapUVy(float v, uint32_t blockID,uint32_t textureWidth, uint32_t textureHeight, uint32_t atlasWidth, uint32_t atlasHeight){
    return ((atlasHeight-1) - ((blockID/textureWidth + (1-v))*(textureWidth-1)))/textureHeight;
}


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

static vec2_s ForwardFaceUV[] = {
    vec2(0.0f,0.0f),
    vec2(0.0f, 1.0f),
    vec2(1.0f,0.0f),
    vec2(1.0f,1.0f)
};

static vec3_s BackFace[]  =  
{
        vec3(-.5f, -.5f, 0.5f),
        vec3(-.5f, .5f, 0.5f),
        vec3(.5f, .5f, 0.5f),
        vec3(.5f, -.5f, 0.5f)
};

static vec2_s BackFaceUV[] = {
    vec2(1.0f,1.0f),
    vec2(1.0f,0.0f),
    vec2(0.0f, 1.0f),
    vec2(0.0f,0.0f),
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


static vec2_s UpFaceUV[] = {
    vec2(0.0f,0.0f),
    vec2(0.0f, 1.0f),
    vec2(1.0f,0.0f),
    vec2(1.0f,1.0f)
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

static vec2_s DownFaceUV[] = {
    vec2(1.0f,1.0f),
    vec2(1.0f,0.0f),
    vec2(0.0f, 1.0f),
    vec2(0.0f,0.0f),
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

static vec2_s LeftFaceUV[] = {
    vec2(0.0f,0.0f),
    vec2(1.0f,0.0f),
    vec2(1.0f,1.0f),
    vec2(0.0f, 1.0f),
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

static vec2_s RightFaceUV[] = {
    vec2(0.0f,0.0f),
    vec2(1.0f,0.0f),
    vec2(1.0f,1.0f),
    vec2(0.0f, 1.0f),
};

static uint32_t RightTris[] = 
{
    20,22,21,20,23,22
};



FaceMesh facemesh_build(vec3_s* vertexData, uint32_t* indiceData, vec2_s* uvData){
    FaceMesh result;
    memcpy(result.vertexData, vertexData, sizeof(vec3_s) * VERTEXES_PER_FACE);
    memcpy(result.uvData, uvData, sizeof(vec2_s) * VERTEXES_PER_FACE);
    memcpy(result.indicesData, indiceData, sizeof(uint32_t) * FACE_INDICES_COUNT);
    return result;
}

void facemesh_copyIndicesData(FaceMesh* mesh, uint32_t* vertices){
    memcpy(vertices, mesh->indicesData, sizeof(uint32_t) * 6);
}


void facemesh_copyVertexData(FaceMesh* mesh,float* vertices){
    for(int j = 0; j < VERTEXES_PER_FACE;j++){
            memcpy(vertices + j*5, &mesh->vertexData[j], sizeof(float) * 3);
            memcpy(vertices + j*5+3, &mesh->uvData[j], sizeof(float) * 2);
    }
}


BlockMesh blockmesh_build(enum BlockID ID){
    BlockMesh result;
    result.id = ID;
    result.faces[FaceOrder_Front] = facemesh_build(ForwardFace, ForwardTris, ForwardFaceUV);
    result.faces[FaceOrder_Back] = facemesh_build(BackFace, BackTris, BackFaceUV);
    result.faces[FaceOrder_Top] = facemesh_build(UpFace, UpTris, UpFaceUV);
    result.faces[FaceOrder_Bottom] = facemesh_build(DownFace, DownTris, DownFaceUV);
    result.faces[FaceOrder_Left] = facemesh_build(LeftFace, LeftTris, LeftFaceUV);
    result.faces[FaceOrder_Right] = facemesh_build(RightFace, RightTris, RightFaceUV);
    for(int i = 0; i < FaceOrder_End;i++){
        for(int j = 0; j < VERTEXES_PER_FACE;j++){
            result.faces[i].uvData[j].x = blockmesh_mapUVx(result.faces[i].uvData[j].x, result.id,TEXTURE_WIDTH, TEXTURE_HEIGHT, ATLAS_WIDTH, ATLAS_HEIGHT);
            result.faces[i].uvData[j].y = blockmesh_mapUVy(result.faces[i].uvData[j].y, result.id,TEXTURE_WIDTH, TEXTURE_HEIGHT, ATLAS_WIDTH, ATLAS_HEIGHT);
        }
    }
    result.vertexCount = N_FACES * VERTEXES_PER_FACE * 3 + N_FACES* VERTEXES_PER_FACE * 2;
    result.indicesCount = FACE_INDICES_COUNT * N_FACES;
    return result;
}

void blockmesh_copyVertexData(BlockMesh *mesh, float* vertices){
    for(int i = 0; i < FaceOrder_End;i++){
        facemesh_copyVertexData(&mesh->faces[i], vertices + (i* VERTEXES_PER_FACE*5));
    }
}

void blockmesh_copyIndicesData(BlockMesh* mesh, uint32_t* indices){
    for(int i = 0; i < FaceOrder_End;i++){
        // memcpy(indices + (i*6), mesh->faces[i].indicesData, sizeof(uint32_t) * 6);
        facemesh_copyIndicesData(&mesh->faces[i], indices + i*6);
    }
}