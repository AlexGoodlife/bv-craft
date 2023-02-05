#include "Chunk.h"
#include "common.h"

#define INDEXCHUNK(x,y,z) INDEX3D(x,y,z,CHUNK_WIDTH,CHUNK_HEIGHT)

#define CHUNK_IN_BOUNDS(x,y,z) ((x) >= 0 && (x) < CHUNK_WIDTH && (y) >= 0 && (y) < CHUNK_HEIGHT && (z) >= 0 && (z) < CHUNK_DEPTH)


ChunkMesh* chunckmesh_init(){
    ChunkMesh* result = malloc(sizeof(ChunkMesh));
    result->faceCount = 0;
    // result->vertices= calloc((CHUNK_WIDTH*CHUNK_DEPTH*CHUNK_HEIGHT)*FLOATS_PER_CUBE,sizeof(float));
    result->vertices = calloc((((CHUNK_DEPTH*CHUNK_HEIGHT*CHUNK_WIDTH)/2) * FLOATS_PER_CUBE), sizeof(float));
    glGenVertexArrays(1,&result->VAO);
    glGenBuffers(1, &result->VBO);
    return result;
}

static ivec3_s check_directions[FaceOrder_End] = 
{
    ivec3(0,0,-1), // FRONT
    ivec3(0,0,1), // BACK
    ivec3(0,1,0), // TOP
    ivec3(0,-1,0), // BOTTOM 
    ivec3(-1,0,0), // LEFT
    ivec3(1,0,0) // RIGHT
};

bool check_face_directions(Chunk* chunk, enum FaceOrder face,uint32_t x,uint32_t y,uint32_t z){
    ivec3_s check_vec = ivec3_add(&check_directions[face], &ivec3(x,y,z));
    if(!CHUNK_IN_BOUNDS(check_vec.x,check_vec.y,check_vec.z))
        return true;
    return chunk->map[INDEXCHUNK(check_vec.x, check_vec.y, check_vec.z)] == 0;
}  


void check_all_directions(Chunk* chunk, uint32_t x, uint32_t y, uint32_t z){
    for(int direction = 0; direction < FaceOrder_End;direction++){
        // LOG("Check %d %d %d\n", x , y, z);
        if(check_face_directions(chunk,direction,x,y,z)){
            facemesh_copyVertexData(
                &all_blocks[chunk->map[INDEXCHUNK(x,y,z)]-1].faces[direction], 
                chunk->mesh->vertices + (chunk->mesh->faceCount * FLOATS_PER_VERTEX * VERTEXES_PER_FACE),
                &vec3(x,y,z)
            );
            chunk->mesh->faceCount++;
        }
    }
}

void chunk_update(Chunk* chunk){
    chunk->mesh->faceCount = 0;
    for(uint32_t z = 0; z < CHUNK_DEPTH; z++){
        for(uint32_t y = 0; y < CHUNK_HEIGHT;y++){
            for(uint32_t x = 0; x < CHUNK_WIDTH;x++){
                if(chunk->map[INDEXCHUNK(x,y,z)] != 0){
                    check_all_directions(chunk,x,y,z);
                }
            }
        }
    }
    // chunk->mesh->vertices = realloc(chunk->mesh->vertices,chunk->mesh->faceCount * FLOATS_PER_VERTEX*VERTEXES_PER_FACE*sizeof(float)); // THIS CRASHES UPDATE IDK WHY
}



Chunk* chunk_build(uint32_t map[CHUNK_DEPTH * CHUNK_WIDTH*CHUNK_HEIGHT]){
    Chunk* result = malloc(sizeof(Chunk));
    result->map = calloc(CHUNK_DEPTH*CHUNK_WIDTH*CHUNK_HEIGHT,sizeof(uint32_t));
    LOG("MADE IT THROUGH calloc\n");
    memcpy(result->map, map, sizeof(uint32_t) * CHUNK_HEIGHT * CHUNK_WIDTH * CHUNK_DEPTH);
    LOG("MADE IT THROUGH MMCPY\n");
    result->mesh = chunckmesh_init();
    LOG("MADE IT THROUGH init\n");
    chunk_update(result);
    LOG("MADE IT THROUGH update\n");
    chunk_prepare(result);
    LOG("MADE IT THROUGH prepare\n");
    return result;
}

void chunk_prepare(Chunk* chunk){

    glBindVertexArray(chunk->mesh->VAO);


    glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, chunk->mesh->faceCount * FLOATS_PER_VERTEX*VERTEXES_PER_FACE *sizeof(float), chunk->mesh->vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    
}

void chunk_render(Chunk* chunk){
    glBindVertexArray(chunk->mesh->VAO);
    glDrawArrays(GL_TRIANGLES,0, chunk->mesh->faceCount*VERTEXES_PER_FACE);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void chunk_destroy(Chunk* chunk){
    glDeleteVertexArrays(1, &chunk->mesh->VAO);
    glDeleteBuffers(1, &chunk->mesh->VBO);
    free(chunk->mesh->vertices);
    free(chunk->mesh);
    free(chunk->map);
    free(chunk);
    
}
