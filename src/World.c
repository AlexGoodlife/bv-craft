#include <errno.h>
#include "World.h"
#include "Chunk.h"
#include "Shader.h"
#include "common.h"
#include "worldgen.h"
#include "math/mat.h"
#include "math/vec.h"
#include <stdlib.h> // malloc
#include <string.h> //memcpy
                    
#define NUMBER_OF_THREADS 8
#define TICK_THROTTLE 4

#define MULTITHREAD 0

vec3_s calculate_bottom_left(vec3_s pos, uint32_t width, uint32_t height){
  float x_offset = (width/2)*CHUNK_WIDTH + CHUNK_WIDTH/2;
  float y_offset = (height/2)*CHUNK_DEPTH + CHUNK_DEPTH/2;
  vec3_s translate_x = vec3_sub(pos, vec3(x_offset, CHUNK_HEIGHT/2,y_offset));
  // translate_x.y = CHUNK_HEIGHT;
  return translate_x;
}

//TODO MAKE NEGATIVE NUMBERS WORK PROPER
ivec2_s world_get_index(World *world, vec3_s pos){
  vec3_s pos_to_center = vec3_sub(pos, world->bottom_left_offset);
  int x = pos_to_center.x / CHUNK_WIDTH;
  int y = pos_to_center.z / CHUNK_DEPTH;
  return ivec2(x, y);
}

void world_draw(World* world, Shader_id shader,vec3_s view_pos, mat4_s projection, mat4_s view){
    shader_use(shader);
    uint32_t chunks_size = world->map_height * world->map_width;
    for(int i = 0; i < chunks_size;i++){
      if(world->chunk_map[i] != NULL && world->chunk_map[i]->is_updated && !world->chunk_map[i]->is_prepared){
        chunk_prepare(world->chunk_map[i]);
      }
    }
    for (int y = 0; y < world->map_height; y++) {
      for (int x = 0; x < world->map_width; x++)  {
        if(world->chunk_map[INDEX2D(x, y, world->map_width)] == NULL) continue;
        if(!world->chunk_map[INDEX2D(x, y, world->map_width)]->is_prepared) continue;
        mat4_s model = init_mat4_id;
        vec3_s translate = vec3((float)(x * CHUNK_WIDTH), 0.0f, (float)(y * CHUNK_DEPTH));
        vec3_s translate_add = vec3_add(translate, world->bottom_left_offset);
        model = linealg_translate(&model, translate_add);
        
        view_pos = vec3_add(view_pos, world->bottom_left_offset);
        // pass transformation matrices to the shader
        shader_setMat4(shader, "projection", &projection);
        shader_setMat4(shader, "model", &model);
        shader_setMat4(shader, "view", &view);
        shader_setVec3(shader, "viewPos",&view_pos);

        chunk_render(world->chunk_map[INDEX2D(x, y, world->map_width)]);
    }

  }
}

void world_generate_chunks(World* world);

World *world_init(uint32_t map_width, uint32_t map_height, vec3_s center_pos) {
  World *result = malloc(sizeof(World));
  result->chunk_map = malloc(sizeof(Chunk *) * map_height * map_width);
  memset(result->chunk_map,0, sizeof(Chunk*)*map_height*map_width);
  // result->chunk_map = memcpy(result->chunk_map, chunk_map,
  //                            sizeof(Chunk *) * map_height * map_width);
  // LOG("WORLD MEMCPY");
  result->map_height = map_height;
  result->map_width = map_width;
  result->center_index = ivec2(map_width/2, map_height/2);
  result->center_coord = center_pos;
  result->bottom_left_offset = calculate_bottom_left(center_pos, result->map_width,result->map_height);
  // LOG_VEC3(result->bottom_left_offset);
  result->throttle_max = TICK_THROTTLE;
  world_generate_chunks(result);
  // uint32_t chunks_size = map_width * map_height;
  // for(int i = 0; i < chunks_size;i++){
  //   result->chunk_map[i] = chunk_build(test_map);  
  // }

  // for(int i = 0; i < chunks_size;i++){
  //   chunk_update(result->chunk_map,result->map_width, result->map_height,i, result->chunk_map[i] );
  // }


  
  return result;
}

void world_destroy(World *world) {
  for (uint32_t y = 0; y < world->map_height; y++) {
    for (uint32_t x = 0; x < world->map_width; x++) {
      chunk_destroy(&world->chunk_map[y * world->map_width + x]);
    }
  }
  free(world);
}

#define STEP_SIZE 10

// Very inneficient raycasting but it works
Raycast_Payload world_raycast(World *world, vec3_s pos, vec3_s direction){
  vec3_s direction_normalize = vec3_normalize(direction);
  direction_normalize.x /= STEP_SIZE;
  direction_normalize.y /= STEP_SIZE;
  direction_normalize.z /= STEP_SIZE;
  Raycast_Payload result = {
    .world_hit = ivec2(-1,-1),
    .chunk_hit = ivec3(-1,-1,-1),
    .hit = false
  };
  vec3_s save = vec3_cpy(pos);
  for(int i = 0; i < RAYCAST_AMOUNT*STEP_SIZE;i++){
    ivec2_s world_pos = world_get_index(world, pos); // Get index of block in the world

    vec3_s offseted_from_bottom = vec3_sub(pos,world->bottom_left_offset); // Offset from the bottom left offset to get acutal world space coordinates

    int chunk_x = round(fmodf(offseted_from_bottom.x, CHUNK_WIDTH));
    int chunk_z = round(fmodf(offseted_from_bottom.z, CHUNK_DEPTH));

    ivec3_s chunk_pos = ivec3(chunk_x, round(offseted_from_bottom.y), chunk_z);
    uint32_t world_index = INDEX2D(world_pos.x, world_pos.y, world->map_width);
    uint32_t chunk_index = INDEXCHUNK(chunk_pos.x, chunk_pos.y, chunk_pos.z);

    if (IN_BOUNDS_2D(world_pos.x, world_pos.y, world->map_width,world->map_height)){
      if(IN_BOUNDS_3D(chunk_pos.x, chunk_pos.y, chunk_pos.z, CHUNK_WIDTH,CHUNK_HEIGHT, CHUNK_DEPTH)) {
        if(world->chunk_map[world_index]->map[chunk_index] != 0){
          result.pos_hit = vec3_cpy(chunk_pos); // Get chunk coordinates
          result.pos_hit = vec3(result.pos_hit.x  + (world_pos.x * CHUNK_WIDTH), result.pos_hit.y , result.pos_hit.z + (world_pos.y * CHUNK_DEPTH)); // Translate coordinates into world space to compare with hit
          result.face_hit = block_intersect(result.pos_hit, vec3_sub(save, world->bottom_left_offset),vec3_sub(pos, world->bottom_left_offset)); // send it to intersection, also offset things to bottom left
          result.world_hit = world_pos;
          result.chunk_hit = chunk_pos;
          result.hit = true;
          return result;
        }
      }
    }

    pos = vec3_add(pos, direction_normalize);

  }
  result.pos_hit = pos;
  return result;

}

typedef struct{
  Chunk* chunk;
  int world_index;
}Chunk_set;

typedef struct{
  Chunk_set* map;
  Chunk** world_map;
  uint32_t map_width;
  uint32_t map_height;
  int start;
  int end;
  int throttle_max;

}Update_Args;

#define LOG_ARGS(args) LOG("map: %p, width: %d, height: %d, start: %d, end: %d\n", args.map, args.map_width, args.map_height, args.start, args.end);

pthread_mutex_t lock;

static void world_update_threads(void* args){
  Update_Args *arguments = (Update_Args*)args;
  int count = 0;
  for(int i = arguments->start; i < arguments->end && count < arguments->throttle_max; i++){
    if(!arguments->map[i].chunk->is_updated){
      chunk_update(
        arguments->world_map,
        arguments->map_width,
        arguments->map_height,
        arguments->map[i].world_index,
        arguments->map[i].chunk
      );
      count++;
    }
  }
}

void world_generate_chunks(World *world){
  // LOG("BOTTOM LEFT");
  // LOG_VEC3(world->bottom_left_offset);
  for(int y = 0; y < world->map_height;y++){
    for(int x = 0; x < world->map_width;x++){
      if(world->chunk_map[INDEX2D(x,y,world->map_width)] == NULL){
        vec3_s pos = vec3_add(vec3(x * CHUNK_WIDTH, 0, y * CHUNK_DEPTH), world->bottom_left_offset);
        uint8_t* map = worldgen_gen_chunk(pos, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH);
        world->chunk_map[INDEX2D(x,y,world->map_width)] = chunk_build(map);
        free(map);
      }
    }
  }
}

void world_update_chunks(World *world, vec3_s new_pos, ivec2_s new_index){
  vec3_s new_bot_left = calculate_bottom_left(new_pos, world->map_width,world->map_height);
  ivec2_s center_offset = ivec2_sub(world->center_index, new_index);
  world->bottom_left_offset = new_bot_left;

  uint32_t width = world->map_width;
  uint32_t height = world->map_height;
  int chunks_size = world->map_width*world->map_height;
  Chunk* old_chunks[chunks_size];

  memcpy(old_chunks, world->chunk_map, sizeof(Chunk*)* chunks_size);
  
  memset(world->chunk_map,0, sizeof(Chunk*)*chunks_size);

  // MOVE OLD CHUNKS TO NEW POSITIONS AND DESTROY OLD ONES
  for(int y = 0; y < height;y++){
    for(int x = 0; x < width;x++){
      ivec2_s new_index = ivec2_add(ivec2(x,y), center_offset); 
      if(IN_BOUNDS_2D(new_index.x, new_index.y, width, height)){
        uint32_t new_index_pos = INDEX2D(new_index.x, new_index.y, width);
        uint32_t old_index_pos = INDEX2D(x, y, width);
        //memcpy(world->chunk_map + new_index_pos, old_chunks + old_index_pos, sizeof(Chunk*));
        world->chunk_map[new_index_pos] = old_chunks[old_index_pos];    
        if(y == 0 || y == height-1 || x == 0 || x == width-1){
          world->chunk_map[new_index_pos]->is_updated = false; // Update every single chunk (necessary?)
        }

        //world->chunk_map[new_index_pos]->is_updated = false; // Update every single chunk (necessary?)
      }
      else{
         chunk_destroy(&old_chunks[INDEX2D(x,y,width)]);
      }
    }

  }

#if 0

  // GENERATE NEW CHUNKS 
  for(int i = 0; i < chunks_size;i++){
    if(world->chunk_map[i] == NULL){
      uint8_t* map = worldgen_random(CHUNK_WIDTH, CHUNK_HEIGHT,CHUNK_DEPTH);
      world->chunk_map[i] = chunk_build(map);
      free(map);
     // world->chunk_map[i] = chunk_build(test_map); 
    }
  }

#else
  world_generate_chunks(world);

#endif
}


void world_update_new_chunks(World* world, Threadpool* pool){
#if !MULTITHREAD
  int count = 0;
  for(int i = 0; i < world->map_width*world->map_height && count < world->throttle_max;i++){
    if(!world->chunk_map[i]->is_updated){
      chunk_update(world->chunk_map, world->map_width, world->map_height,i,world->chunk_map[i]);
      count++;
    }
  }
#else
  errno = 0;
  uint32_t chunks_size = world->map_height*world->map_width;

  //Lazy fix to always update the center chunk
  if(!world->chunk_map[chunks_size/2]->is_updated){
    chunk_update(world->chunk_map, world->map_width, world->map_width, chunks_size/2, world->chunk_map[chunks_size/2]);
  }

  Chunk_set chunks_to_update[chunks_size];
  int n_chunks_to_update = 0;

  for(int i = 0; i < chunks_size;i++){
    if(!world->chunk_map[i]->is_updated){
      chunks_to_update[n_chunks_to_update++] = (Chunk_set){.chunk = world->chunk_map[i], .world_index = i};
    }
  }
  // pthread_t threads[NUMBER_OF_THREADS];
  Update_Args args[NUMBER_OF_THREADS];
  // Task *tasks[NUMBER_OF_THREADS];
  Task tasks[NUMBER_OF_THREADS];
  int n_chunks = ceil((double)(n_chunks_to_update) / NUMBER_OF_THREADS);
  int used_args = 0;
  for(int i = 0; i < NUMBER_OF_THREADS;i++){
    used_args++;
    int start = i*n_chunks;
    int end = start + n_chunks;
    end = MIN(end, n_chunks_to_update);
    Update_Args arg ={
      .map_width = world->map_width,
      .map_height = world->map_height,
      .world_map = world->chunk_map,
      .map = chunks_to_update,
      .end = end,
      .start = start,
      .throttle_max = world->throttle_max,
    };
    args[i] = arg;
    tasks[i].arg = args + i;
    tasks[i].func = &world_update_threads;
    threadpool_add_task(pool,tasks + i);
    if(end == n_chunks_to_update)break;
  }
  threadpool_wait(pool);
#endif
}

void world_check_center(World* world, vec3_s pos){
  ivec2_s new_index = world_get_index(world, pos);
  ivec2_s new_index_offset = ivec2_sub(world->center_index, new_index);
  world->center_coord= vec3_sub(world->center_coord, vec3(new_index_offset.x*CHUNK_WIDTH, 0 , new_index_offset.y*CHUNK_DEPTH));
  // vec3_s new_center = vec3_sub(world->bottom_left_offset, vec3(new_index_offset.x*CHUNK_WIDTH, 0 , new_index_offset.y*CHUNK_DEPTH));
  uint32_t new_index_pos = INDEX2D(new_index.x, new_index.y, world->map_width);
  if(new_index_pos != ((world->map_height * world->map_width) / 2)){
    world_update_chunks(world, world->center_coord, new_index);
  }

}
void world_update(World* world,vec3_s pos, Threadpool* pool){
   world_update_new_chunks(world,pool); 
   world_check_center(world, pos);

}

static vec3_s check_directions[FaceOrder_End] = 
{
    vec3(0,0,-1), // FRONT
    vec3(0,0,1), // BACK
    vec3(0,1,0), // TOP
    vec3(0,-1,0), // BOTTOM 
    vec3(-1,0,0), // LEFT
    vec3(1,0,0) // RIGHT
};

static void world_hit_check_neighbours(World *world, uint32_t world_index, Raycast_Payload* raycast){
    //Update surrounding chunks
    if(raycast->chunk_hit.x == CHUNK_WIDTH-1 || raycast->chunk_hit.x == 0){
        if(IN_BOUNDS_2D(raycast->world_hit.x +1,raycast->world_hit.y, world->map_width, world->map_height))
          world->chunk_map[world_index + 1]->is_updated = false;
        if(IN_BOUNDS_2D(raycast->world_hit.x - 1,raycast->world_hit.y, world->map_width, world->map_height))
          world->chunk_map[world_index - 1]->is_updated = false;
    }
    if(raycast->chunk_hit.z == CHUNK_DEPTH-1 || raycast->chunk_hit.z == 0){
        if(IN_BOUNDS_2D(raycast->world_hit.x,raycast->world_hit.y+1, world->map_width, world->map_height))
          world->chunk_map[world_index + world->map_width]->is_updated = false;
        if(IN_BOUNDS_2D(raycast->world_hit.x,raycast->world_hit.y-1, world->map_width, world->map_height))
          world->chunk_map[world_index - world->map_width]->is_updated = false;
    }

}

void world_break_block(World* world, Raycast_Payload raycast){
  if(!raycast.hit) return;

  uint32_t world_index = INDEX2D(raycast.world_hit.x, raycast.world_hit.y, world->map_width);
  uint32_t chunk_index = INDEXCHUNK(raycast.chunk_hit.x, raycast.chunk_hit.y,raycast.chunk_hit.z);
  uint32_t block_id = world->chunk_map[world_index]->map[chunk_index];
  if(block_id != 0){
    world->chunk_map[world_index]->map[chunk_index] = 0;
    world->chunk_map[world_index]->is_updated = false;
    world_hit_check_neighbours(world, world_index, &raycast);

  }
}

void world_place_block(World * world, Raycast_Payload raycast){
    if(!raycast.hit) return;

    uint32_t world_index = INDEX2D(raycast.world_hit.x, raycast.world_hit.y, world->map_width);
    vec3_s new_chunk_vec = vec3_add(vec3_cpy(raycast.chunk_hit), check_directions[raycast.face_hit]);
    uint32_t chunk_index = INDEXCHUNK(new_chunk_vec.x, new_chunk_vec.y,new_chunk_vec.z);
    if(IN_BOUNDS_3D(new_chunk_vec.x, new_chunk_vec.y, new_chunk_vec.z, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH)){
      if(world->chunk_map[world_index]->map[chunk_index] == 0){
        world->chunk_map[world_index]->map[chunk_index] = Gravel + 1;
        world->chunk_map[world_index]->is_updated = false;
        world_hit_check_neighbours(world, world_index, &raycast);
      }
    }
}
