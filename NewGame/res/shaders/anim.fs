#version 330 core

out vec4 fragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;

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
uniform DirectionalLight directionalLight;

vec3 CalcDirectionalLighting(DirectionalLight light, vec3 color, vec3 normal, vec3 viewDir)
{
    // Diffuse shading.
    vec3 lightDir = normalize(-light.direction);
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseFactor * color;

    // Specular shading.
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * specularFactor * color;

    return (diffuse + specular);
}

void main()
{
    vec3 color = texture(texture_diffuse1, texCoords).rgb;
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    // Directional lighting.
    vec3 result = CalcDirectionalLighting(directionalLight, color, norm, viewDir);

    // Ambient lighting.
    vec3 ambient = directionalLight.ambient * color;

    // Blinn-Phong lighting.
    result += ambient;
    fragColor = vec4(result, 1.0);
}