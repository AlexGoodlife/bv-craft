#include "World.h"
#include <stdlib.h> // malloc
#include <string.h> //memcpy

World* world_init(Chunk** chunk_map, uint32_t map_width, uint32_t map_height){
    World* result = malloc(sizeof(World));
    result->chunk_map = malloc(sizeof(Chunk*) * map_height * map_width);
    LOG("%p %p\n", chunk_map[0], chunk_map[1]);
    result->chunk_map = memcpy(result->chunk_map, chunk_map, sizeof(Chunk*) * map_height*map_width);
    LOG("WORLD MEMCPY");
    result->map_height = map_height;
    result->map_width = map_width;
    for(uint32_t y = 0; y < map_height; y++){
        for(uint32_t x = 0; x < map_width;x++){
            chunk_update(result->chunk_map, result->map_width, result->map_height, INDEX2D(x,y,map_width),result->chunk_map[INDEX2D(x,y,map_width)]);
            chunk_prepare(result->chunk_map[INDEX2D(x,y,map_width)]);
        }
    }
    return result;
}

void world_destroy(World* world){
    for(uint32_t y = 0; y < world->map_height;y++){
        for(uint32_t x = 0; x < world->map_width;x++){
            chunk_destroy(world->chunk_map[y*world->map_width + x]);
        }
    }
    free(world);
}

void world_render(World* world){
    for(uint32_t y = 0; y < world->map_height;y++){
        for(uint32_t x = 0; x < world->map_width;x++){
            chunk_render(world->chunk_map[y*world->map_width + x]);
        }
    }
}


