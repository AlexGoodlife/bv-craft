#ifndef BV_WORLD_H
#define BV_WORLD_H

#include "Chunk.h"

typedef struct
{
    Chunk** chunk_map;    // THIS IS TWO DIMENSIONAL FOR NOW
    uint32_t chunk_count;
}World;

World* world_init(Chunk** chunk_map, uint32_t chunk_count);





#endif