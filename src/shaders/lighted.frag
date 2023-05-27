#version 330 core
out vec4 FragColor;  

in vec3 FragPos;
in vec2 UV;
in vec3 Normal;

uniform sampler2D Atlas;
uniform float shininess;
uniform vec3 viewPos;

struct DirLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirlight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(Atlas, UV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(Atlas, UV));
    vec3 specular = light.specular * spec * vec3(texture(Atlas, UV));
    return (ambient + diffuse + specular);
}  

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 norm = normalize(Normal);

    vec3 result = CalcDirLight(dirlight, norm,viewDir);

  vec3 new_normal;
  new_normal.x = Normal.x * 0.5 + 0.5;
  new_normal.y = Normal.y * 0.5 + 0.5;
  new_normal.z = Normal.z * 0.5 + 0.5;

   FragColor = vec4(result, 1.0f);
  FragColor = vec4(new_normal, 1.0f);
}
