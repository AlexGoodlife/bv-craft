#include "Chunk.h"


ChunkMesh* chunckmesh_init(){
    ChunkMesh* result = malloc(sizeof(ChunkMesh));
    // result->cubeCount = cubeCount;
    result->cubeCount = 0;
    glGenVertexArrays(1,&result->VAO);
    glGenBuffers(1, &result->VBO);
    return result;
}

void chunk_update(Chunk* chunk){
    for(uint32_t z = 0; z < CHUNK_DEPTH; z++){
        for(uint32_t y = 0; y < CHUNK_HEIGHT;y++){
            for(uint32_t x = 0; x < CHUNK_WIDTH;x++){
                if(chunk->map[z][y*CHUNK_WIDTH + x] > 0){
                    blockmesh_copyVertexData(&all_blocks[chunk->map[z][y*CHUNK_WIDTH + x]], chunk->mesh->vertices + (chunk->mesh->cubeCount * (FLOATS_PER_CUBE)), &vec3(x,y,z));
                    chunk->mesh->cubeCount++;
                }
            }
        }
    }
}

Chunk* chunk_build(uint32_t map[CHUNK_DEPTH][CHUNK_WIDTH*CHUNK_HEIGHT]){
    Chunk* result = malloc(sizeof(Chunk));
    for(uint32_t z = 0; z < CHUNK_DEPTH;z++){
        memcpy(result->map[z], map[z], sizeof(uint32_t) * CHUNK_WIDTH*CHUNK_HEIGHT);
    }
    result->mesh = chunckmesh_init();
    chunk_update(result);
    chunk_prepare(result);
    printf("CUBES: %d\n", result->mesh->cubeCount);
    return result;
}

void chunk_prepare(Chunk* chunk){
    glBindVertexArray(chunk->mesh->VAO);


    glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chunk->mesh->vertices), chunk->mesh->vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    
}

void chunk_render(Chunk* chunk){
    glBindVertexArray(chunk->mesh->VAO);
    glDrawArrays(GL_TRIANGLES,0, chunk->mesh->cubeCount*VERTEXES_PER_FACE*N_FACES);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void chunk_destroy(Chunk* chunk){
    glDeleteVertexArrays(1, &chunk->mesh->VAO);
    glDeleteBuffers(1, &chunk->mesh->VBO);
    free(chunk->mesh);
    free(chunk);
    
}
