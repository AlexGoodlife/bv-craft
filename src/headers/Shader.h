#ifndef BV_SHADER_H
#define BV_SHADER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common.h"


typedef GLuint Shader_id;

Shader_id shader_load(const char* vertexPath, const char* fragPath);

void shader_destroy(Shader_id id);

void shader_use(Shader_id id);

#endif

