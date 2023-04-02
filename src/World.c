#include "World.h"
#include "common.h"
#include <stdlib.h> // malloc
#include <string.h> //memcpy

World *world_init(Chunk **chunk_map, uint32_t map_width, uint32_t map_height, vec3_s center_offset) {
  World *result = malloc(sizeof(World));
  result->chunk_map = malloc(sizeof(Chunk *) * map_height * map_width);
  // LOG("%p %p\n", chunk_map[0], chunk_map[1]);
  result->chunk_map = memcpy(result->chunk_map, chunk_map,
                             sizeof(Chunk *) * map_height * map_width);
  // LOG("WORLD MEMCPY");
  result->map_height = map_height;
  result->map_width = map_width;
  for (uint32_t y = 0; y < map_height; y++) {
    for (uint32_t x = 0; x < map_width; x++) {
      chunk_update(result->chunk_map, result->map_width, result->map_height,
                   INDEX2D(x, y, map_width),
                   result->chunk_map[INDEX2D(x, y, map_width)]);
      chunk_prepare(result->chunk_map[INDEX2D(x, y, map_width)]);
    }
  }
  result->center_offset = center_offset;
  return result;
}

void world_destroy(World *world) {
  for (uint32_t y = 0; y < world->map_height; y++) {
    for (uint32_t x = 0; x < world->map_width; x++) {
      chunk_destroy(world->chunk_map[y * world->map_width + x]);
    }
  }
  free(world);
}

void world_render(World *world) {
  for (uint32_t y = 0; y < world->map_height; y++) {
    for (uint32_t x = 0; x < world->map_width; x++) {
      chunk_render(world->chunk_map[y * world->map_width + x]);
    }
  }
}

bool world_raycast(World *world, vec3_s pos, vec3_s direction,
                   vec3_s world_scaling, ivec2_s *out_world_coord,
                   ivec3_s *out_chunk_coord) 
{
  vec3_s current_pos = vec3(pos.x, pos.y, pos.z);
  vec3_s direction_normalize = vec3_normalize(&direction);
  LOGLN(" ");
  LOG_VEC3(direction_normalize);
  for (int i = 0; i < RAYCAST_AMOUNT; i++) {
    ivec2_s world_pos = ivec2(current_pos.x / CHUNK_WIDTH, current_pos.z / CHUNK_DEPTH);

    int chunk_x = (int)(current_pos.x) % CHUNK_WIDTH;
    int chunk_z = (int)current_pos.z % CHUNK_DEPTH;

    ivec3_s chunk_pos = ivec3(chunk_x, current_pos.y, chunk_z);
    uint32_t world_index = INDEX2D(world_pos.x, world_pos.y, world->map_width);
    uint32_t chunk_index = INDEXCHUNK(chunk_pos.x, chunk_pos.y, chunk_pos.z);
    
    LOG("%d %d\n", world_index, chunk_index);
    LOG_VEC3(current_pos);
    LOG_IVEC3(chunk_pos);

    if (IN_BOUNDS_2D(world_pos.x, world_pos.y, world->map_width,world->map_height)){
      if(IN_BOUNDS_3D(chunk_pos.x, chunk_pos.y, chunk_pos.z, CHUNK_WIDTH,CHUNK_HEIGHT, CHUNK_DEPTH)) {
        if(world->chunk_map[world_index]->map[chunk_index] != 0){
          *out_world_coord = ivec2_cpy(world_pos);
          *out_chunk_coord = ivec3_cpy(chunk_pos);
          return true;
        }
      }
    }
    current_pos = vec3_add(&current_pos, &direction_normalize);
  }
  return false;
}
