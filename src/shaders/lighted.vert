#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec2 UV;
out vec3 Normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos ,1.0f);
    Normal = aNormal;
    //Normal = mat3(transpose(inverse(model)))* aNormal;
    FragPos = vec3(model * vec4(aPos,1.0f));
    UV = aUV;
}       
