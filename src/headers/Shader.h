
#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef GLuint Shader_id;

Shader_id loadShader(const char* vertexPath, const char* fragPath);

void destroyShader(Shader_id id);

