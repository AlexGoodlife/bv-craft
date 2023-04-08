#ifndef BV_WORLD_H
#define BV_WORLD_H

#include "Chunk.h"
#include "common.h"
#include "math/vec.h"
#include <pthread.h>

#define RAYCAST_AMOUNT 5

extern uint32_t *test_map; // TEMPORARY TEST_MAP 

typedef struct{
    Chunk** chunk_map;    // THIS IS TWO DIMENSIONAL FOR NOW
    uint32_t map_width;
    uint32_t map_height;
    ivec2_s center_index;
    vec3_s bottom_left_offset;
    vec3_s center_coord;
    int throttle_max;
}World;

World* world_init(Chunk** chunk_map, uint32_t map_width, uint32_t map_height, vec3_s center_pos);
void world_draw(World* world, Shader_id shader, mat4_s projection, mat4_s view);

void world_destroy(World* world);
ivec2_s world_get_index(World *world, vec3_s pos);
void world_update(World* world,vec3_s pos);

// DEPRECATED
bool world_raycast(World* world, vec3_s pos, vec3_s direction,vec3_s world_scaling, ivec2_s* out_world_coord, ivec3_s* out_chunk_coord);


#endif
