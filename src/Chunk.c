#include "Chunk.h"
#include "common.h"
#include <stdint.h>


#define CHUNK_IN_BOUNDS(x,y,z) ((x) >= 0 && (x) < CHUNK_WIDTH && (y) >= 0 && (y) < CHUNK_HEIGHT && (z) >= 0 && (z) < CHUNK_DEPTH)


ChunkMesh* chunckmesh_init(){
    ChunkMesh* result = malloc(sizeof(ChunkMesh));
    result->faceCount = 0;
    // result->vertices = calloc((((CHUNK_DEPTH*CHUNK_HEIGHT*CHUNK_WIDTH)/2) * FLOATS_PER_CUBE), sizeof(float));
    result->vertices = NULL;
    glGenVertexArrays(1,&result->VAO);
    glGenBuffers(1, &result->VBO);
    return result;
}

void chunkmesh_destroy(ChunkMesh* mesh){
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    free(mesh->vertices);
    free(mesh);
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

bool check_neighbouring_chunks(Chunk** map, uint32_t map_width, uint32_t map_height, Chunk* chunk, uint32_t chunk_pos, enum FaceOrder face,int x,int y,int z){
    int map_y = chunk_pos/map_width;
    int map_x = chunk_pos % map_width;
    map_x += check_directions[face].x;
    map_y += check_directions[face].z;
    if(y >= CHUNK_HEIGHT ) return true;
    if(!IN_BOUNDS_2D(map_x,map_y,map_width, map_height) || y < 0)
        return false;
    // if(!IN_BOUNDS_2D(map_x,map_y,map_width, map_height) || y < 0 || y >= CHUNK_HEIGHT)
    //     return true;
    x = (uint32_t)x % CHUNK_WIDTH;
    z = (uint32_t)z % CHUNK_DEPTH;
    return map[INDEX2D(map_x, map_y, map_width)]->map[INDEXCHUNK(x,y,z)] == 0;
}

bool check_face_directions(Chunk** map, uint32_t map_width, uint32_t map_height, Chunk* chunk, uint32_t chunk_pos, enum FaceOrder face,int x,int y,int z){
    ivec3_s check_vec = ivec3_add(check_directions[face], ivec3(x,y,z));
    if(!CHUNK_IN_BOUNDS(check_vec.x,check_vec.y,check_vec.z)){
        return check_neighbouring_chunks(map,map_width,map_height,chunk,chunk_pos,face,check_vec.x,check_vec.y,check_vec.z);
    }
    return chunk->map[INDEXCHUNK(check_vec.x, check_vec.y, check_vec.z)] == 0;
}  


void check_all_directions(Chunk** map, uint32_t map_width, uint32_t map_height,Chunk* chunk, uint32_t chunk_pos,int x, int y, int z){
    for(int direction = 0; direction < FaceOrder_End;direction++){
        // LOG("Check %d %d %d\n", x , y, z);
        if(check_face_directions(map,map_width,map_height,chunk,chunk_pos,direction,x,y,z)){
            facemesh_copyVertexData(
                &all_blocks[chunk->map[INDEXCHUNK(x,y,z)]-1].faces[direction], 
                chunk->mesh->vertices + (chunk->mesh->faceCount * FLOATS_PER_VERTEX * VERTEXES_PER_FACE),
                vec3((float)x,(float)y,(float)z)
            );
            chunk->mesh->faceCount++;
        }
    }
}

void chunk_update(Chunk** map, uint32_t map_width, uint32_t map_height,uint32_t chunk_pos,Chunk* chunk){
    free(chunk->mesh->vertices); // I Think I can remove this
    chunk->mesh->vertices = calloc((((CHUNK_DEPTH*CHUNK_HEIGHT*CHUNK_WIDTH)/2) * FLOATS_PER_CUBE), sizeof(float));
    chunk->mesh->faceCount = 0;
    chunk->is_prepared = false;
    for(uint32_t z = 0; z < CHUNK_DEPTH; z++){
        for(uint32_t y = 0; y < CHUNK_HEIGHT;y++){
            for(uint32_t x = 0; x < CHUNK_WIDTH;x++){
                if(chunk->map[INDEXCHUNK(x,y,z)] != 0){
                    check_all_directions(map, map_width,map_height,chunk,chunk_pos,x,y,z);
                }
            }
        }
    }
    chunk->is_updated = true;
    // chunk->mesh->vertices = realloc(chunk->mesh->vertices,chunk->mesh->faceCount * FLOATS_PER_VERTEX*VERTEXES_PER_FACE*sizeof(float)); // THIS CRASHES UPDATE IDK WHY
}


Chunk* chunk_build(uint8_t map[CHUNK_DEPTH * CHUNK_WIDTH*CHUNK_HEIGHT]){
    Chunk* result = malloc(sizeof(Chunk));
    result->map = calloc(CHUNK_DEPTH*CHUNK_WIDTH*CHUNK_HEIGHT,sizeof(uint8_t));
    memcpy(result->map, map, sizeof(uint8_t) * CHUNK_HEIGHT * CHUNK_WIDTH * CHUNK_DEPTH);
    result->mesh = chunckmesh_init();
    result->is_updated = false;
    result->is_prepared = false;
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

    chunk->is_prepared = true;
    free(chunk->mesh->vertices);
    chunk->mesh->vertices= NULL;
    
}


void chunk_render(Chunk* chunk){
    glBindVertexArray(chunk->mesh->VAO);
    glDrawArrays(GL_TRIANGLES,0, chunk->mesh->faceCount*VERTEXES_PER_FACE);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}


void chunk_destroy(Chunk** chunk){
    chunkmesh_destroy((*chunk)->mesh);
    free((*chunk)->map);
    free((*chunk));
    *chunk = NULL;
    
}
