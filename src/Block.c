#include "Block.h"

BlockMesh* all_blocks;

float blockmesh_mapUVx(float u, uint32_t blockID,uint32_t textureWidth, uint32_t textureHeight, uint32_t atlasWidth, uint32_t atlasHeight){
    return (float)((int)((textureWidth)*(blockID + u)) % atlasWidth) /256;
}
float blockmesh_mapUVy(float v, uint32_t blockID,uint32_t textureWidth, uint32_t textureHeight, uint32_t atlasWidth, uint32_t atlasHeight){
    return ((atlasHeight) - (((blockID/textureHeight) + (1-v))*(textureHeight))) / 256;
}


static vec3_s ForwardFace[]  =  
{
    vec3(-0.5f, -0.5f, -0.5f), // Bottom left
    vec3(-0.5f, 0.5f, -0.5f), // top left
    vec3(0.5f, -0.5f, -0.5f), // Bottom right
    vec3(-0.5f, 0.5f, -0.5f), // top left
    vec3(0.5f, 0.5f, -0.5f), // top right
    vec3(0.5f, -0.5f, -0.5f) // Bottom right
};


static vec2_s ForwardFaceUV[] = {
    vec2(0.0f,0.0f), // bottom left
    vec2(0.0f, 1.0f), // top left
    vec2(1.0f,0.0f), // bottom right
    vec2(0.0f, 1.0f), // top left
    vec2(1.0f,1.0f), // top right
    vec2(1.0f,0.0f), // bottom right
};

static vec3_s BackFace[]  =  
{
    vec3(-.5f, -.5f, 0.5f), // Bottom left // looking from the inside
    vec3(-.5f, .5f, 0.5f), // Top left
    vec3(.5f, -0.5f, 0.5f), // Bottom right
    vec3(-.5f, .5f, 0.5f), // Top left
    vec3(.5f, .5f, 0.5f), // Top right 
    vec3(.5f, -0.5f, 0.5f)// Bottom right
};

static vec2_s BackFaceUV[] = {
    vec2(1.0f,0.0f), // bottom left
    vec2(1.0f, 1.0f), // top left
    vec2(0.0f,0.0f), // bottom right
    vec2(1.0f, 1.0f), // top left
    vec2(0.0f,1.0f), // top right
    vec2(0.0f,0.0f) // bottom right
};


static vec3_s UpFace[]  =  
{
    vec3(-.5f, .5f, -0.5f), // Bottom left
    vec3(-.5f, .5f, .5f), // Top left
    vec3(.5f, .5f, -.5f), // Bottom right
    vec3(-.5f, .5f, .5f), // Top left
    vec3(.5f, .5f, .5f), // Top right
    vec3(.5f, .5f, -.5f) // Bottom right
};


static vec2_s UpFaceUV[] = {
    vec2(0.0f,0.0f), // bottom left
    vec2(0.0f, 1.0f), // top left
    vec2(1.0f,0.0f), // bottom right
    vec2(0.0f, 1.0f), // top left
    vec2(1.0f,1.0f), // top right
    vec2(1.0f,0.0f), // bottom right
};


static vec3_s DownFace[]  =  
{
    vec3(-.5f, -.5f, .5f), // Bottom left
    vec3(-.5f, -.5f, -.5f), // Top left
    vec3(.5f, -.5f, .5f), // Bottom Right
    vec3(-.5f, -.5f, -.5f), // Top left
    vec3(.5f, -.5f, -.5f), // Top Right
    vec3(.5f, -.5f, .5f), // Bottom Right
};

static vec2_s DownFaceUV[] = {
    vec2(0.0f,0.0f), // bottom left
    vec2(0.0f, 1.0f), // top left
    vec2(1.0f,0.0f), // bottom right
    vec2(0.0f, 1.0f), // top left
    vec2(1.0f,1.0f), // top right
    vec2(1.0f,0.0f) // bottom right
};

static vec3_s LeftFace[]  = 
{
    vec3(-.5f, -.5f, .5f), // Bottom left
    vec3(-.5f, .5f, .5f), // Top left
    vec3(-.5f, -.5f, -.5f), // Bottom right
    vec3(-.5f, .5f, .5f), // Top left
    vec3(-.5f, .5f, -.5f), // Top right
    vec3(-.5f, -.5f, -.5f) // Bottom right
};

static vec2_s LeftFaceUV[] = {
    vec2(0.0f,0.0f), // Bottom left
    vec2(0.0f, 1.0f), // Top left
    vec2(1.0f,0.0f), //Bottom right
    vec2(0.0f, 1.0f), // Top left
    vec2(1.0f,1.0f), // Top right
    vec2(1.0f,0.0f) //Bottom right
};

static vec3_s RightFace[]  =
{
    vec3(0.5f, -0.5f, -0.5f), // Bottom left
    vec3(0.5f, 0.5f, -0.5f), // Top left
    vec3(0.5f, -0.5f, 0.5f), // Bottom Right
    vec3(0.5f, 0.5f, -0.5f), // Top left
    vec3(0.5f, 0.5f, 0.5f), // Top right
    vec3(0.5f, -0.5f, 0.5f) // Bottom Right
};

static vec2_s RightFaceUV[] = {
    vec2(0.0f,0.0f), // Bottom left
    vec2(0.0f,1.0f), // Top left
    vec2(1.0f,0.0f), // Bottom Right
    vec2(0.0f,1.0f), // Top left
    vec2(1.0f, 1.0f), // Top right
    vec2(1.0f,0.0f), // Bottom Right
};


// FaceMesh facemesh_build(vec3_s* vertexData, uint32_t* indiceData, vec2_s* uvData){
//     FaceMesh result;
//     memcpy(result.vertexData, vertexData, sizeof(vec3_s) * VERTEXES_PER_FACE);
//     memcpy(result.uvData, uvData, sizeof(vec2_s) * VERTEXES_PER_FACE);
//     memcpy(result.indicesData, indiceData, sizeof(uint32_t) * FACE_INDICES_COUNT);
//     return result;
// }
FaceMesh facemesh_build(vec3_s* vertexData, vec2_s* uvData, uint32_t texture_pos_offset){
    FaceMesh result;
    result.texture_pos_offset = texture_pos_offset;
    memcpy(result.vertexData, vertexData, sizeof(vec3_s) * VERTEXES_PER_FACE);
    memcpy(result.uvData, uvData, sizeof(vec2_s) * VERTEXES_PER_FACE);
    return result;
}

void facemesh_copyVertexData(FaceMesh* mesh,float* vertices, vec3_s* positionOffset){
    for(int j = 0; j < VERTEXES_PER_FACE;j++){
        // mesh->vertexData[j] = vec3_add(&mesh->vertexData[j], positionOffset);
        vec3_s offseted = vec3_add(&mesh->vertexData[j],positionOffset);
        // printf("%f %f %f\n", mesh->vertexData[j].x, mesh->vertexData[j].y, mesh->vertexData[j].z);
        memcpy(vertices + j*5, &offseted, sizeof(float) * 3);
        memcpy(vertices + j*5+3, &mesh->uvData[j], sizeof(float) * 2);
    }
}


BlockMesh blockmesh_build(enum BlockID ID){
    BlockMesh result;
    result.id = ID;
    result.faces[FaceOrder_Front] = facemesh_build(ForwardFace,ForwardFaceUV, 0);
    result.faces[FaceOrder_Back] = facemesh_build(BackFace, BackFaceUV,0);
    result.faces[FaceOrder_Top] = facemesh_build(UpFace,UpFaceUV,0);
    result.faces[FaceOrder_Bottom] = facemesh_build(DownFace, DownFaceUV,0);
    result.faces[FaceOrder_Left] = facemesh_build(LeftFace, LeftFaceUV,0);
    result.faces[FaceOrder_Right] = facemesh_build(RightFace, RightFaceUV,0);
    float half_pixel_size = ((1/ATLAS_WIDTH)* 0.5f);
    for(int i = 0; i < FaceOrder_End;i++){
        for(int j = 0; j < VERTEXES_PER_FACE;j++){
            result.faces[i].uvData[j].x = blockmesh_mapUVx(result.faces[i].uvData[j].x, result.id + result.faces[i].texture_pos_offset,TEXTURE_WIDTH, TEXTURE_HEIGHT, ATLAS_WIDTH, ATLAS_HEIGHT);
            result.faces[i].uvData[j].y = blockmesh_mapUVy(result.faces[i].uvData[j].y, result.id + result.faces[i].texture_pos_offset,TEXTURE_WIDTH, TEXTURE_HEIGHT, ATLAS_WIDTH, ATLAS_HEIGHT);
        }
        result.faces[i].uvData[0].x += half_pixel_size;
        result.faces[i].uvData[0].y += half_pixel_size;

        result.faces[i].uvData[1].x += half_pixel_size;
        result.faces[i].uvData[1].y -= half_pixel_size;

        result.faces[i].uvData[2].x -= half_pixel_size;
        result.faces[i].uvData[2].y += half_pixel_size;

        result.faces[i].uvData[3].x += half_pixel_size;
        result.faces[i].uvData[3].y -= half_pixel_size;

        result.faces[i].uvData[4].x -= half_pixel_size;
        result.faces[i].uvData[4].y -= half_pixel_size;

        result.faces[i].uvData[5].x -= half_pixel_size;
        result.faces[i].uvData[5].y += half_pixel_size;
    }
    result.vertexCount = N_FACES * VERTEXES_PER_FACE * 3 + N_FACES* VERTEXES_PER_FACE * 2;
    result.indicesCount = FACE_INDICES_COUNT * N_FACES;
    return result;
}

void blockmesh_copyVertexData(BlockMesh *mesh, float* vertices, vec3_s* positionOffset){
    // printf("%f %f %f\n", positionOffset->x, positionOffset->y, positionOffset->z);
    for(int i = 0; i < FaceOrder_End;i++){
        facemesh_copyVertexData(&mesh->faces[i], vertices + (i* VERTEXES_PER_FACE*FLOATS_PER_VERTEX),positionOffset);
    }
}


BlockMesh blockmesh_build_block(enum BlockID ID,uint32_t texture_atlas_position, uint32_t* face_texture_offsets){
    BlockMesh result;
    result.id = ID;
    result.texture_atlas_position = texture_atlas_position;
    result.faces[FaceOrder_Front] = facemesh_build(ForwardFace,ForwardFaceUV, face_texture_offsets[FaceOrder_Front]);
    result.faces[FaceOrder_Back] = facemesh_build(BackFace, BackFaceUV,face_texture_offsets[FaceOrder_Back]);
    result.faces[FaceOrder_Top] = facemesh_build(UpFace,UpFaceUV,face_texture_offsets[FaceOrder_Top]);
    result.faces[FaceOrder_Bottom] = facemesh_build(DownFace, DownFaceUV,face_texture_offsets[FaceOrder_Bottom]);
    result.faces[FaceOrder_Left] = facemesh_build(LeftFace, LeftFaceUV,face_texture_offsets[FaceOrder_Left]);
    result.faces[FaceOrder_Right] = facemesh_build(RightFace, RightFaceUV,face_texture_offsets[FaceOrder_Right]);
    float half_pixel_size = ((1/ATLAS_WIDTH)* 0.5f);
    for(int i = 0; i < FaceOrder_End;i++){
        for(int j = 0; j < VERTEXES_PER_FACE;j++){
            result.faces[i].uvData[j].x = blockmesh_mapUVx(result.faces[i].uvData[j].x, result.id + result.faces[i].texture_pos_offset,TEXTURE_WIDTH, TEXTURE_HEIGHT, ATLAS_WIDTH, ATLAS_HEIGHT);
            result.faces[i].uvData[j].y = blockmesh_mapUVy(result.faces[i].uvData[j].y, result.id + result.faces[i].texture_pos_offset,TEXTURE_WIDTH, TEXTURE_HEIGHT, ATLAS_WIDTH, ATLAS_HEIGHT);
        }
        result.faces[i].uvData[0].x += half_pixel_size;
        result.faces[i].uvData[0].y += half_pixel_size;

        result.faces[i].uvData[1].x += half_pixel_size;
        result.faces[i].uvData[1].y -= half_pixel_size;

        result.faces[i].uvData[2].x -= half_pixel_size;
        result.faces[i].uvData[2].y += half_pixel_size;

        result.faces[i].uvData[3].x += half_pixel_size;
        result.faces[i].uvData[3].y -= half_pixel_size;

        result.faces[i].uvData[4].x -= half_pixel_size;
        result.faces[i].uvData[4].y -= half_pixel_size;

        result.faces[i].uvData[5].x -= half_pixel_size;
        result.faces[i].uvData[5].y += half_pixel_size;
    }
    result.vertexCount = N_FACES * VERTEXES_PER_FACE * 5;
    result.indicesCount = FACE_INDICES_COUNT * N_FACES;
    return result;
}

void blockmesh_buildAllBlocks(){
    all_blocks = malloc(sizeof(BlockMesh) * Block_count);

    // Now we build these suckers one by one

    //Gravel block
    all_blocks[Gravel] = blockmesh_build_block(Gravel, 0, (uint32_t[]){0,0,0,0,0,0});
    all_blocks[Stone] = blockmesh_build_block(Stone, 1, (uint32_t[]){0,0,0,0,0,0});

}