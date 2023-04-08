#include <errno.h>
#include <pthread.h>
#include "World.h"
#include "Chunk.h"
#include "Shader.h"
#include "common.h"
#include "math/mat.h"
#include "math/vec.h"
#include <stdlib.h> // malloc
#include <string.h> //memcpy
                    
#define N_THREADS 6


vec3_s calculate_bottom_left(vec3_s pos, uint32_t width, uint32_t height){
  float x_offset = (width/2)*CHUNK_WIDTH + CHUNK_WIDTH/2;
  float y_offset = (height/2)*CHUNK_HEIGHT + CHUNK_HEIGHT/2;
  vec3_s translate_x = vec3_sub(pos, vec3(x_offset, 0,y_offset));
  translate_x.y = 0;
  return translate_x;
}

//TODO MAKE NEGATIVE NUMBERS WORK PROPER
ivec2_s world_get_index(World *world, vec3_s pos){
  vec3_s pos_to_center = vec3_sub(pos, world->bottom_left_offset);
  int x = pos_to_center.x / CHUNK_WIDTH;
  int y = pos_to_center.z / CHUNK_DEPTH;
  return ivec2(x, y);
  //return INDEX2D(x, y, world->map_width);
}

void world_draw(World* world, Shader_id shader, mat4_s projection, mat4_s view){
    shader_use(shader);
    for (int y = 0; y < world->map_height; y++) {
      for (int x = 0; x < world->map_width; x++)  {
        mat4_s model = init_mat4_id;
        vec3_s translate = vec3(x * CHUNK_WIDTH, 0.0f, y * CHUNK_DEPTH);
        vec3_s translate_add = vec3_add(translate, world->bottom_left_offset);
        model = linealg_translate(
            &model, translate_add);

        // pass transformation matrices to the shader
        shader_setMat4(shader, "projection", &projection);
        shader_setMat4(shader, "model", &model);
        shader_setMat4(shader, "view", &view);

        chunk_render(world->chunk_map[INDEX2D(x, y, world->map_width)]);
    }

  }
}

World *world_init(Chunk **chunk_map, uint32_t map_width, uint32_t map_height, vec3_s center_pos) {
  World *result = malloc(sizeof(World));
  result->chunk_map = malloc(sizeof(Chunk *) * map_height * map_width);
  result->chunk_map = memcpy(result->chunk_map, chunk_map,
                             sizeof(Chunk *) * map_height * map_width);
  // LOG("WORLD MEMCPY");
  result->map_height = map_height;
  result->map_width = map_width;
  result->center_index = ivec2(map_width/2, map_height/2);
  result->center_coord = center_pos;
  uint32_t chunks_size = map_width * map_height;
  for(int i = 0; i < chunks_size;i++){
    chunk_update(result->chunk_map,result->map_width, result->map_height,i, result->chunk_map[i] );
    chunk_prepare(result->chunk_map[i]);
  }

  result->bottom_left_offset = calculate_bottom_left(center_pos, result->map_width,result->map_height);
  LOG_VEC3(result->bottom_left_offset);
  
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

bool world_raycast(World *world, vec3_s pos, vec3_s direction,
                   vec3_s world_scaling, ivec2_s *out_world_coord,
                   ivec3_s *out_chunk_coord) 
{
  TODO("IMPLEMENT RAYCAST");
  vec3_s current_pos = vec3(pos.x, pos.y, pos.z);
  vec3_s direction_normalize = vec3_normalize(direction);
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
    current_pos = vec3_add(current_pos, direction_normalize);
  }
  return false;
}

typedef struct{
  Chunk** map;
  uint32_t map_width;
  uint32_t map_height;
  int start;
  int end;

}Update_Args;

#define LOG_ARGS(args) LOG("map: %p, width: %d, height: %d, start: %d, end: %d\n", args.map, args.map_width, args.map_height, args.start, args.end);

pthread_mutex_t lock;

static void* world_update_threads(void* args){
  Update_Args *arguments = (Update_Args*)args;
  Update_Args logger = *arguments;

  for(int i = arguments->start; i < arguments->end; i++){
    chunk_update(
      arguments->map,
      arguments->map_width,
      arguments->map_height,
      i,
      arguments->map[i]
    );
  }
  // pthread_exit(NULL);
  return NULL;
}


void world_update_chunks(World *world, vec3_s new_pos, ivec2_s new_index){
  vec3_s new_bot_left = calculate_bottom_left(new_pos, world->map_width,world->map_height);
  ivec2_s center_offset = ivec2_sub(world->center_index, new_index);

  int chunks_size = world->map_width*world->map_height;
  Chunk* old_chunks[chunks_size];
  memcpy(old_chunks, world->chunk_map, sizeof(Chunk*)* chunks_size);
  
  memset(world->chunk_map,0, sizeof(Chunk*)*world->map_width*world->map_height);

  // MOVE OLD CHUNKS TO NEW POSITIONS 
  for(int y = 0; y < world->map_height;y++){
    for(int x = 0; x < world->map_width;x++){
      ivec2_s new_index = ivec2_sub(ivec2(x,y), center_offset); 
      if(IN_BOUNDS_2D(new_index.x, new_index.y, world->map_width, world->map_height)){
        uint32_t new_index_pos = INDEX2D(new_index.x, new_index.y, world->map_width);
        uint32_t old_index_pos = INDEX2D(x, y, world->map_width);
        //memcpy(world->chunk_map + new_index_pos, old_chunks + old_index_pos, sizeof(Chunk*));
        world->chunk_map[new_index_pos] = old_chunks[old_index_pos];    
      }
      else{
        chunk_destroy(old_chunks[INDEX2D(x,y,world->map_width)]);
      }
    }

  }

  // GENERATE NEW CHUNKS AND DESTROY OLD ONES
  for(int i = 0; i < chunks_size;i++){
    if(world->chunk_map[i] == NULL){
     world->chunk_map[i] = chunk_build(test_map); 
    }
  }

  
  // // UPDATE THE CHUNKS
  // for(int i = 0; i < chunks_size;i++){
  //   chunk_update(world->chunk_map,world->map_width, world->map_height,i, world->chunk_map[i]);
  //   chunk_prepare(world->chunk_map[i]);
  // }
  errno = 0;
  // pthread_mutex_init(&lock, NULL);
  pthread_t threads[N_THREADS];
  Update_Args args[N_THREADS];
  int n_chunks = ceil((double)(chunks_size) / N_THREADS);
  printf("%d\n", n_chunks);
  for(int i = 0; i < N_THREADS;i++){
    int start = i*n_chunks;
    int end = start + n_chunks;
    end = MIN(end, chunks_size);
    Update_Args arg ={
      .map_width = world->map_width,
      .map_height = world->map_height,
      .map = world->chunk_map,
      .end = end,
      .start = start
    };
    args[i] = arg;
    int err = pthread_create(
      threads + i,
      NULL,
      &world_update_threads,
      args + i
    );
    if(err){
      ERROR("PTHREAD ERROR\n");
    }
    if(end == chunks_size)break;
  }

  for(int i = 0; i<  N_THREADS;i++){
    pthread_join(threads[i], NULL);
  }

  for(int i = 0; i < chunks_size;i++){
    chunk_prepare(world->chunk_map[i]);
  }

  world->bottom_left_offset = new_bot_left;
}

void world_update(World* world,vec3_s pos){
  ivec2_s new_index = world_get_index(world, pos);
  ivec2_s new_index_offset = ivec2_sub(world->center_index, new_index);
  world->center_coord= vec3_sub(world->center_coord, vec3(new_index_offset.x*CHUNK_WIDTH, 0 , new_index_offset.y*CHUNK_DEPTH));
  vec3_s new_center = vec3_sub(world->bottom_left_offset, vec3(new_index_offset.x*CHUNK_WIDTH, 0 , new_index_offset.y*CHUNK_DEPTH));
  uint32_t new_index_pos = INDEX2D(new_index.x, new_index.y, world->map_width);
  if(new_index_pos != ((world->map_height * world->map_width) / 2)){
    LOG_VEC3(new_center);
    LOG("INDEX: %d %d\n", new_index.x, new_index.y);
    LOG("%d %d\n", new_index_offset.x, new_index_offset.y);
    world_update_chunks(world, world->center_coord, new_index);
    // world_update_chunks(world, pos, new_index);
  }
  
}
