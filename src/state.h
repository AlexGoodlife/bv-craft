#ifndef BV_STATE_H
#define BV_STATE_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"
#include "Block.h"


#define VSYNC

#define BACKGROUND_COLOR_R 0.2f
#define BACKGROUND_COLOR_G 0.3f
#define BACKGROUND_COLOR_B 0.3f
#define BACKGROUND_COLOR_A 1.0f
#define BACKGROUND_COLOR_4C BACKGROUND_COLOR_R, BACKGROUND_COLOR_G,BACKGROUND_COLOR_B,BACKGROUND_COLOR_A 



typedef struct{
    GLFWwindow* window;
    Camera* camera;
    const char* windowTitle;
    int windowWidth;
    int windowHeight;
}State;

extern Shader_id shader; // temp
extern State* state;
extern float deltaTime;


int init(const char* windowTitle, int windowWidth, int windowHeight);
void processInput(float deltaTime);
void displayFPS(float deltaTime, float lastFrame);
GLuint loadTexture(const char* path);

#endif
