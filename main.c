#include <assert.h>
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

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define WORLD_WIDTH 55
#define WORLD_HEIGHT 55

// #define SCALING_FACTOR vec3(0.5f, 0.5f, 0.5f)

World *world;
uint8_t* test_map;

void run() {

  assert(WORLD_HEIGHT % 2 != 0);
  assert(WORLD_WIDTH % 2 != 0);

  GLuint Atlas_texture = loadTextureArray("resources/big_ass_atlas.png");
  glCheckError();

  vec3_s center = vec3(0,0,0);

  world = world_init(WORLD_WIDTH, WORLD_HEIGHT, center);

  shader_use(shader);
  shader_setInt(shader, "Atlas", 0);
  glCheckError();

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
    glBindTexture(GL_TEXTURE_2D_ARRAY, Atlas_texture);

    if((currentFrame - tickTime)*1000 >= 50){ // 20 ticks per second 1000/ticks
      tickTime = currentFrame;
      world_update(world, state->camera->pos, state->thread_pool);
    }
    world_draw(world, shader ,state->camera->pos,projection, camera_getViewMat(state->camera));

    glfwPollEvents();
    glfwSwapBuffers(state->window);
  }
  world_destroy(world);
  free(test_map);
  shader_destroy(shader);
  cleanup();
}

int main(int argc, char **argv) {
  if (!init("Testing", WINDOW_WIDTH, WINDOW_HEIGHT))
    return -1;

  run();

 return 0;
}
