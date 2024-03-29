#include "Block.h"
#include "stdbool.h"
#include "common.h"

BlockMesh* all_blocks;

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
    vec2(0.0f,1.0f), // bottom left
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,1.0f), // bottom right
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,0.0f), // top right
    vec2(1.0f,1.0f), // bottom right
};


static vec3_s BackFace[]  =  
{
    vec3(-0.5f, -0.5f, 0.5f), // Bottom left // looking from the inside
    vec3(-0.5f, 0.5f, 0.5f), // Top left
    vec3(0.5f, -0.5f, 0.5f), // Bottom right
    vec3(-0.5f, 0.5f, 0.5f), // Top left
    vec3(0.5f, 0.5f, 0.5f), // Top right 
    vec3(0.5f, -0.5f, 0.5f)// Bottom right
};

static vec2_s BackFaceUV[] = {
    vec2(0.0f,1.0f), // bottom left
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,1.0f), // bottom right
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,0.0f), // top right
    vec2(1.0f,1.0f), // bottom right
};


static vec3_s UpFace[]  =  
{
    vec3(-0.5f, 0.5f, -0.5f), // Bottom left
    vec3(-0.5f, 0.5f, 0.5f), // Top left
    vec3(0.5f, 0.5f, -0.5f), // Bottom right
    vec3(-0.5f, 0.5f, 0.5f), // Top left
    vec3(0.5f, 0.5f, 0.5f), // Top right
    vec3(0.5f, 0.5f, -0.5f) // Bottom right
};


static vec2_s UpFaceUV[] = {
    vec2(0.0f,1.0f), // bottom left
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,1.0f), // bottom right
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,0.0f), // top right
    vec2(1.0f,1.0f), // bottom right
};


static vec3_s DownFace[]  =  
{
    vec3(-0.5f, -0.5f, 0.5f), // Bottom left
    vec3(-0.5f, -0.5f, -0.5f), // Top left
    vec3(0.5f, -0.5f, 0.5f), // Bottom Right
    vec3(-0.5f, -0.5f, -0.5f), // Top left
    vec3(0.5f, -0.5f, -0.5f), // Top Right
    vec3(0.5f, -0.5f, 0.5f), // Bottom Right
};

static vec2_s DownFaceUV[] = {
    vec2(0.0f,1.0f), // bottom left
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,1.0f), // bottom right
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,0.0f), // top right
    vec2(1.0f,1.0f), // bottom right
};

static vec3_s LeftFace[]  = 
{
    vec3(-0.5f, -0.5f, 0.5f), // Bottom left
    vec3(-0.5f, 0.5f, 0.5f), // Top left
    vec3(-0.5f, -0.5f, -0.5f), // Bottom right
    vec3(-0.5f, 0.5f, 0.5f), // Top left
    vec3(-0.5f, 0.5f, -0.5f), // Top right
    vec3(-0.5f, -0.5f, -0.5f) // Bottom right
};

static vec2_s LeftFaceUV[] = {
    vec2(0.0f,1.0f), // bottom left
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,1.0f), // bottom right
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,0.0f), // top right
    vec2(1.0f,1.0f), // bottom right
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
    vec2(0.0f,1.0f), // bottom left
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,1.0f), // bottom right
    vec2(0.0f, 0.0f), // top left
    vec2(1.0f,0.0f), // top right
    vec2(1.0f,1.0f), // bottom right
};

static vec3_s NormalData[FaceOrder_End] = 
{
    vec3(0.0f,0.0f,-1.0f), // FRONT
    vec3(0.0f,0.0f,1.0f), // BACK
    vec3(0.0f,1.0f,0.0f), // TOP
    vec3(0.0f,-1.0f,0.0f), // BOTTOM 
    vec3(-1.0f,0.0f,0.0f), // LEFT
    vec3(1.0f,0.0f,0.0f) // RIGHT
};

FaceMesh facemesh_build(enum FaceOrder face,vec3_s* vertexData, vec2_s* uvData, int texture_pos_offset){
    FaceMesh result;
    result.texture_pos_offset = texture_pos_offset;
    memcpy(result.vertexData, vertexData, sizeof(vec3_s) * VERTEXES_PER_FACE);
    memcpy(result.uvData, uvData, sizeof(vec2_s) * VERTEXES_PER_FACE);
    // memcpy(&result.normal, NormalData + face, sizeof(vec3_s));
    result.normal = NormalData[face];
    return result;
}

void facemesh_copyVertexData(FaceMesh* mesh,float* vertices, vec3_s positionOffset){
    for(int j = 0; j < VERTEXES_PER_FACE;j++){
        vec3_s offseted = vec3_add(mesh->vertexData[j],positionOffset);
        float pos = (float)mesh->texture_pos_offset;
        memcpy(vertices + j*FLOATS_PER_VERTEX,   &offseted,        sizeof(float) * 3);
        memcpy(vertices + j*FLOATS_PER_VERTEX+3, &mesh->uvData[j], sizeof(float) * 2);
        memcpy(vertices + j*FLOATS_PER_VERTEX+5, &mesh->normal,    sizeof(float) * 3);
        memcpy(vertices + j*FLOATS_PER_VERTEX+8, &pos,             sizeof(float) * 1);
    }
}


// void blockmesh_copyVertexData(BlockMesh *mesh, float* vertices, vec3_s positionOffset){
//     for(int i = 0; i < FaceOrder_End;i++){
//         facemesh_copyVertexData(&mesh->faces[i], vertices + (i* VERTEXES_PER_FACE*FLOATS_PER_VERTEX),positionOffset);
//     }
// }

BlockMesh blockmesh_build_block(enum BlockID ID,int texture_atlas_position, int* face_texture_offsets){
    BlockMesh result;
    result.id = ID;
    result.texture_atlas_position = texture_atlas_position;
    result.faces[FaceOrder_Front]  =  facemesh_build(FaceOrder_Front,  ForwardFace,  ForwardFaceUV, texture_atlas_position + face_texture_offsets[FaceOrder_Front]);
    result.faces[FaceOrder_Back]   =  facemesh_build(FaceOrder_Back,   BackFace,     BackFaceUV,    texture_atlas_position + face_texture_offsets[FaceOrder_Back]);
    result.faces[FaceOrder_Top]    =  facemesh_build(FaceOrder_Top,    UpFace,       UpFaceUV,      texture_atlas_position + face_texture_offsets[FaceOrder_Top]);
    result.faces[FaceOrder_Bottom] =  facemesh_build(FaceOrder_Bottom, DownFace,     DownFaceUV,    texture_atlas_position + face_texture_offsets[FaceOrder_Bottom]);
    result.faces[FaceOrder_Left]   =  facemesh_build(FaceOrder_Left,   LeftFace,     LeftFaceUV,    texture_atlas_position + face_texture_offsets[FaceOrder_Left]);
    result.faces[FaceOrder_Right]  =  facemesh_build(FaceOrder_Right,  RightFace,    RightFaceUV,   texture_atlas_position + face_texture_offsets[FaceOrder_Right]);

    result.vertexCount = N_FACES * VERTEXES_PER_FACE * FLOATS_PER_VERTEX;
    result.indicesCount = FACE_INDICES_COUNT * N_FACES;
    return result;
}

void blockmesh_buildAllBlocks(){
    all_blocks = malloc(sizeof(BlockMesh) * Block_count);

    // Now we build these suckers one by one

    all_blocks[Gravel] = blockmesh_build_block(Gravel, 0, (int[]){0,0,0,0,0,0});
    all_blocks[Snow] = blockmesh_build_block(Snow, 1, (int[]){0,0,171,49,0,0});
    all_blocks[Grass] = blockmesh_build_block(Grass, 2, (int[]){1,1,0,48,1,1});
    all_blocks[Water] = blockmesh_build_block(Water, 416, (int[]){0,0,0,0,0,0});
    all_blocks[Dirt] = blockmesh_build_block(Dirt, 50, (int[]){0,0,0,0,0,0});
    all_blocks[Stone] = blockmesh_build_block(Stone, 19, (int[]){0,0,0,0,0,0});
}

static vec3_s check_directions[FaceOrder_End] = 
{
    vec3(0,0,-1), // FRONT
    vec3(0,0,1), // BACK
    vec3(0,1,0), // TOP
    vec3(0,-1,0), // BOTTOM 
    vec3(-1,0,0), // LEFT
    vec3(1,0,0) // RIGHT
};

bool is_inside_face(enum FaceOrder face, vec3_s hit_pos, vec3_s face_center){
    bool check_x = hit_pos.x <= face_center.x + 0.5 && hit_pos.x >= face_center.x - 0.5; 
    bool check_y = hit_pos.y <= face_center.y + 0.5 && hit_pos.y >= face_center.y - 0.5; 
    bool check_z = hit_pos.z <= face_center.z + 0.5 && hit_pos.z >= face_center.z - 0.5; 
    return check_x && check_y && check_z;

}

enum FaceOrder block_intersect(vec3_s block_pos, vec3_s ray_origin,vec3_s ray_end){
    // Since we are dealing with blocks we can make alot of assumptions about their characteristics :)

    vec3_s direction = (vec3_sub(ray_end, ray_origin));
    // direction = vec3_normalize(direction);
    for(int i = 0; i < FaceOrder_End;i++){
        vec3_s face_center = vec3_add(block_pos,vec3_mult_const(check_directions[i], 0.5));
        vec3_s normal = vec3_normalize(vec3_sub(face_center,block_pos)); // the normal is just the center of the plane - center of block
        float denom = vec3_dot(normal, (direction));
        if(ABS(denom) > 0.0001f){
            float t = vec3_dot(vec3_sub(face_center, ray_origin), normal) /denom;
            if(t >= 0 && t <= 1){
                vec3_s hit_point = vec3_add(ray_origin, vec3_mult_const(direction, t));
                if(is_inside_face(i,hit_point, face_center)) return i;
            }
        }
    }
    return FaceOrder_Miss;
}
