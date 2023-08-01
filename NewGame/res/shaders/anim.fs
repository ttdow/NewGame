#version 330 core

out vec4 fragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // Sample diffuse texture.
    vec3 color = texture(texture_diffuse1, texCoords).rgb;

    // Ambient lighting.
    vec3 ambient = light.ambient * color;

    // Diffuse lighting.
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * color;

    // Specular lighting.
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, texCoords).rgb;

    // Blinn-Phong lighting.
    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}