#ifndef BV_WORLD_H
#define BV_WORLD_H

#include "Chunk.h"
#include "common.h"

typedef struct{
    Chunk** chunk_map;    // THIS IS TWO DIMENSIONAL FOR NOW
    uint32_t map_width;
    uint32_t map_height;
}World;

World* world_init(Chunk** chunk_map, uint32_t map_width, uint32_t map_height);

void world_render(World* world);
void world_destroy(World* world);


#endif