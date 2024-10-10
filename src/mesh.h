#ifndef BV_MESH_H
#define BV_MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/Shader.h"

typedef struct{
  GLuint VAO, VBO;
  float *vertices;
  GLuint* textures;
  int triangle_count;
}Mesh;

void mesh_init(float* vertices, GLuint* textures);

void mesh_render(Shader_id shader);

#endif // BV_MESH_H
