#version 460 core
out vec4 FragColor;  

in vec3 Pos;
in vec2 UV;
in vec3 Normal;
in float index;

uniform sampler2DArray Atlas;

void main()
{
  FragColor = texture(Atlas, vec3(UV, index));
}

