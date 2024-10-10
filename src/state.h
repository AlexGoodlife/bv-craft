#ifndef BV_STATE_H
#define BV_STATE_H
#include <glad/glad.h>
#include "Block.h"
#include "Camera.h"
#include "Shader.h"
#include "World.h"
#include "thread.h"
#include <GLFW/glfw3.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "worldgen.h"

#define VSYNC 0
#define SCALING_FACTOR vec3(0.5f, 0.5f, 0.5f)

#define N_THREADS 8
#define MSAA_SAMPLES 16

#define TEXTURE_WIDTH 16
#define TEXTURE_HEIGHT 16
#define ATLAS_WIDTH 512
#define ATLAS_HEIGHT 256

#define BACKGROUND_COLOR_R 0.0f
#define BACKGROUND_COLOR_G 181.0f
#define BACKGROUND_COLOR_B 226.0f
#define BACKGROUND_COLOR_A 1.0f
#define BACKGROUND_COLOR_4C                                                    \
  BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B, BACKGROUND_COLOR_A

extern World *world;

typedef struct {
  GLFWwindow *window;
  Camera *camera;
  const char *windowTitle;
  int windowWidth;
  int windowHeight;
  Threadpool* thread_pool;
} State;

extern Shader_id shader; // temp
extern State *state;
extern float deltaTime;

int init(const char *windowTitle, int windowWidth, int windowHeight);
void close();
void processInput(float deltaTime);
void displayFPS(float deltaTime, float lastFrame);
GLuint loadTexture(const char *path);
GLuint loadTextureArray(const char* path);
#endif
