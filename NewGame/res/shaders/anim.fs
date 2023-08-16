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

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float intensity;
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;

vec3 CalcDirectionalLighting(DirectionalLight light, vec3 color, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // Diffuse shading.
    float diffuseFactor = max(dot(normal, halfwayDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseFactor * color;

    // Specular shading.
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * specularFactor * color;

    return (diffuse + specular);
}

vec3 CalcPointLighting(PointLight light, vec3 fragPos, vec3 color, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // Attenuation.
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Diffuse shading.
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseFactor * color * attenuation;
    diffuse = light.diffuse * diffuseFactor * vec3(1.0, 0.0, 0.0) * attenuation;

    // Specular shading.
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * specularFactor * color * attenuation;

    // Ambient shading.
    vec3 ambient = light.ambient * color * attenuation;

    // Flickering effect.
    vec3 result = light.intensity * (ambient + diffuse + specular);

    return result;
}

void main()
{
    vec3 color = texture(texture_diffuse1, texCoords).rgb;
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    // Directional lighting.
    vec3 result = CalcDirectionalLighting(directionalLight, color, norm, viewDir);
    result += CalcPointLighting(pointLight, fragPos, vec3(1.0, 0.0, 0.0), norm, viewDir);

    // Ambient lighting.
    vec3 ambient = directionalLight.ambient * color;

    // Blinn-Phong lighting.
    result += ambient;
    fragColor = vec4(result, 1.0);
}