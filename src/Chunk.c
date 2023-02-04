#include "Chunk.h"


ChunkMesh* chunckmesh_init(){
    ChunkMesh* result = malloc(sizeof(ChunkMesh));
    // result->cubeCount = cubeCount;
    result->faceCount = 0;
    result->vertices= calloc((CHUNK_WIDTH*CHUNK_DEPTH*CHUNK_HEIGHT)*FLOATS_PER_CUBE,sizeof(float));
    glGenVertexArrays(1,&result->VAO);
    glGenBuffers(1, &result->VBO);
    return result;
}

// bool check_face_directions(Chunk* chunk,uint32_t x,uint32_t y,uint32_t z){

bool check_face_directions(Chunk* chunk, enum FaceOrder face,uint32_t x,uint32_t y,uint32_t z){
    bool success = false;
    switch (face)
    {
    case FaceOrder_Front:
        if(z == 0)
            success = true;
        else if(chunk->map[z-1][y*CHUNK_WIDTH + x] == 0)
            success = true;
        break;
    case FaceOrder_Back:
         if(z == CHUNK_DEPTH-1)
            success = true;
        else if(chunk->map[z+1][y*CHUNK_WIDTH + x] == 0)
            success = true;
        break;
    case FaceOrder_Top:
        if(y == CHUNK_HEIGHT-1)
            success = true;
        else if(chunk->map[z][(y+1)*CHUNK_WIDTH + x] == 0)
            success = true;
        break;
    case FaceOrder_Bottom:
         if(y == 0)
            success = true;
        else if(chunk->map[z][(y-1)*CHUNK_WIDTH + x] == 0)
            success = true;
        break;
    case FaceOrder_Left:
        if(x == 0)
            success = true;
        else if(chunk->map[z][(y)*CHUNK_WIDTH + x-1] == 0)
            success = true;
        break;
    case FaceOrder_Right:
        if(x == CHUNK_WIDTH-1)
            success = true;
        else if(chunk->map[z][(y)*CHUNK_WIDTH + x+1] == 0)
            success = true;
        break;
    
    default:
        break;
    }
   
    return success;
}   

void check_all_directions(Chunk* chunk, uint32_t x, uint32_t y, uint32_t z){
    for(int direction = 0; direction < FaceOrder_End;direction++){
        if(check_face_directions(chunk,direction,x,y,z)){
            facemesh_copyVertexData(
                &all_blocks[chunk->map[z][y*CHUNK_WIDTH + x]-1].faces[direction], 
                chunk->mesh->vertices + (chunk->mesh->faceCount * FLOATS_PER_VERTEX * VERTEXES_PER_FACE),
                &vec3(x,y,z)
            );
            chunk->mesh->faceCount++;
        }
    }
}

void chunk_update(Chunk* chunk){
    for(uint32_t z = 0; z < CHUNK_DEPTH; z++){
        for(uint32_t y = 0; y < CHUNK_HEIGHT;y++){
            for(uint32_t x = 0; x < CHUNK_WIDTH;x++){
                if(chunk->map[z][y*CHUNK_WIDTH + x] != 0){
                    check_all_directions(chunk,x,y,z);
                }
            }
        }
    }
    chunk->mesh->vertices = realloc(chunk->mesh->vertices,chunk->mesh->faceCount * FLOATS_PER_VERTEX*VERTEXES_PER_FACE*sizeof(float));
}



Chunk* chunk_build(uint32_t map[CHUNK_DEPTH][CHUNK_WIDTH*CHUNK_HEIGHT]){
    Chunk* result = malloc(sizeof(Chunk));
    result->map = malloc(sizeof(uint32_t*)*CHUNK_DEPTH);
    for(uint32_t z = 0; z < CHUNK_DEPTH;z++){
        result->map[z] = malloc(sizeof(uint32_t) * CHUNK_WIDTH * CHUNK_HEIGHT);
        memcpy(result->map[z], map[z], sizeof(uint32_t) * CHUNK_WIDTH*CHUNK_HEIGHT);
    }
    printf("Here\n");
    result->mesh = chunckmesh_init();
    chunk_update(result);
    chunk_prepare(result);
    // printf("CUBES: %d\n", result->mesh->cubeCount);
    return result;
}

void chunk_prepare(Chunk* chunk){
    glBindVertexArray(chunk->mesh->VAO);


    glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh->VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(chunk->mesh->vertices), chunk->mesh->vertices, GL_STATIC_DRAW);
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
    for(int i = 0; i < CHUNK_DEPTH;i++){
        free(chunk->map[i]);
    }
    free(chunk->map);
    free(chunk);
    
}
