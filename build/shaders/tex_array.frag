#version 460 core
out vec4 FragColor;  

in vec3 Pos;
in vec3 FragPos;
in vec2 UV;
in vec3 Normal;
in float index;

uniform sampler2DArray Atlas;

uniform float shininess;
uniform vec3 viewPos;

struct DirLight{
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform DirLight dirlight;

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(-light.direction);
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

  // combine results
  vec4 ambient  = vec4(light.ambient,1.0f)  * texture(Atlas, vec3(UV, index));
  vec4 diffuse  = vec4(light.diffuse,1.0f)  * diff * texture(Atlas, vec3(UV, index));
  vec4 specular = vec4(light.specular,1.0f) * spec * texture(Atlas, vec3(UV, index));
  return (ambient + diffuse + specular);
}  

void main()
{
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 norm = normalize(Normal);
  vec4 result = CalcDirLight(dirlight, norm,viewDir);
  FragColor = result;
}

