#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#include "src/Block.h"
#include "src/Camera.h"
#include "src/Chunk.h"
#include "src/World.h"
#include "src/math/linealg.h"
#include "src/math/vec.h"
#include "src/state.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define WORLD_WIDTH 33
#define WORLD_HEIGHT 33

// #define SCALING_FACTOR vec3(0.5f, 0.5f, 0.5f)
//

World *world;
uint8_t* test_map;

void run() {

  // uint32_t test_map[CHUNK_DEPTH *CHUNK_HEIGHT*CHUNK_WIDTH];
  test_map = malloc(sizeof(uint8_t) * CHUNK_DEPTH * CHUNK_WIDTH * CHUNK_HEIGHT);
  for (uint32_t z = 0; z < CHUNK_DEPTH; z++) {
    for (uint32_t y = 0; y < CHUNK_HEIGHT; y++) {
      for (uint32_t x = 0; x < CHUNK_WIDTH; x++) {
        test_map[z * CHUNK_WIDTH * CHUNK_HEIGHT + y * CHUNK_WIDTH + x] = 3;
      }
    }
  }

  test_map[0 + 0 + 0] = 0;

  for (int i = 0; i < CHUNK_HEIGHT; i++) {
    test_map[CHUNK_DEPTH / 2 * CHUNK_WIDTH * CHUNK_HEIGHT + i * CHUNK_WIDTH +
             CHUNK_WIDTH / 2] = 0;
  }

  test_map[CHUNK_DEPTH / 2 * CHUNK_WIDTH * CHUNK_HEIGHT + 3 * CHUNK_WIDTH +
           CHUNK_WIDTH / 2 - 1] = 0;

  test_map[CHUNK_DEPTH / 2 * CHUNK_WIDTH * CHUNK_HEIGHT +
           (CHUNK_HEIGHT - 2) * CHUNK_WIDTH + CHUNK_WIDTH / 2] = 1;

  GLuint Atlas_texture = loadTexture("resources/big_ass_atlas.png");

  Chunk *map[WORLD_HEIGHT * WORLD_WIDTH];
  for(int i = 0; i < WORLD_HEIGHT * WORLD_WIDTH;i++){
    map[i] = chunk_build(test_map);
  }
  // Chunk *map[WORLD_HEIGHT * WORLD_WIDTH] = {test, test2, test3, test4};

  // vec3_s offset = vec3_cpy(state->camera->pos);
  vec3_s center = vec3(0,0,0);

   world = world_init(map, WORLD_WIDTH, WORLD_HEIGHT, center);


  shader_use(shader);
  shader_setInt(shader, "Atlas", 0);

  // float waitTime = 0.0f;
  mat4_s projection = init_mat4_id;
  projection = linealg_perspective((float)state->windowWidth /(float)state->windowHeight,radians(state->camera->fov), 0.1f, 1000.0f);

  float tickTime = 0;
  while (!glfwWindowShouldClose(state->window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    displayFPS(deltaTime, lastFrame);

    processInput(deltaTime);

    glClearColor(BACKGROUND_COLOR_4C);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Atlas_texture);

    if((currentFrame - tickTime)*1000 >= 50){ // 20 ticks per second 1000/ticks
      tickTime = currentFrame;
      world_update(world, state->camera->pos, state->thread_pool);
    }
    world_draw(world, shader, projection, camera_getViewMat(state->camera));

    glfwPollEvents();
    glfwSwapBuffers(state->window);
  }
  world_destroy(world);
  free(test_map);
  // chunk_destroy(test);
  shader_destroy(shader);
  close();
}

int main(int argc, char **argv) {
  if (!init("Testing", WINDOW_WIDTH, WINDOW_HEIGHT))
    return -1;

  run();

  return 0;
}
