#ifndef BV_SHADER_H
#define BV_SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "common.h"
#include "math/linealg.h"

typedef GLuint Shader_id;

Shader_id shader_load(const char* vertexPath, const char* fragPath);

void shader_destroy(Shader_id id);

void shader_use(Shader_id id);

void shader_setBool(Shader_id ID, const char* name, bool value);
void shader_setInt(Shader_id ID, const char* name, int value);
void shader_setFloat(Shader_id ID, const char* name, float value);
void shader_setVec2(Shader_id ID, const char* name, vec2_s* v);
void shader_setVec3(Shader_id ID, const char* name, vec3_s*v);
void shader_setVec4(Shader_id ID, const char* name, vec4_s*v);
void shader_setMat2(Shader_id ID, const char* name, mat2_s*m);
void shader_setMat3(Shader_id ID, const char* name, mat3_s*m);
void shader_setMat4(Shader_id ID, const char* name, mat4_s*m);

#endif

