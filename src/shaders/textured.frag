#version 330 core
out vec4 FragColor;  

in vec3 Pos;
in vec2 UV;
in vec3 Normal;

uniform sampler2D Atlas;

void main()
{
    FragColor = vec4(vec3(texture(Atlas, UV)), 1.0f);
}
