#ifndef BV_WORLD_H
#define BV_WORLD_H

#include "Chunk.h"
#include "common.h"
#include "math/vec.h"
#include "thread.h"
// #include "thread.h"

#define RAYCAST_AMOUNT 5

extern uint8_t *test_map; // TEMPORARY TEST_MAP 

typedef struct{
    Chunk** chunk_map;    // THIS IS TWO DIMENSIONAL FOR NOW
    uint32_t map_width;
    uint32_t map_height;
    ivec2_s center_index;
    vec3_s bottom_left_offset;
    vec3_s center_coord;
    int throttle_max;
}World;

typedef struct {
    bool hit;
    vec3_s pos_hit;
    ivec2_s world_hit;
    ivec3_s chunk_hit;
    enum FaceOrder face_hit;
}Raycast_Payload;

World* world_init(uint32_t map_width, uint32_t map_height, vec3_s center_pos);
void world_draw(World* world, Shader_id shader, mat4_s projection, mat4_s view);

void world_destroy(World* world);
ivec2_s world_get_index(World *world, vec3_s pos);
void world_update(World* world,vec3_s pos, Threadpool* pool);

Raycast_Payload world_raycast(World *world, vec3_s pos, vec3_s direction);
void world_place_block(World * world, Raycast_Payload raycast);
void world_break_block(World * world, Raycast_Payload raycast);

#endif
