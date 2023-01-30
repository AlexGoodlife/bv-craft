#ifndef BV_SHADER_H
#define BV_SHADER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common.h"


typedef GLuint Shader_id;

Shader_id loadShader(const char* vertexPath, const char* fragPath);

inline void destroyShader(Shader_id id);

inline void useShader(Shader_id id);

#endif

