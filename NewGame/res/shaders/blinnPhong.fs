#version 330 core

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

struct LightComponent
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec2 texCoords;
in vec3 normal;
in vec3 worldPos;
in vec4 fragPosLightSpace;

// Bloom.
uniform float bloomBrightness;

// Gamma correction.
uniform bool firstGamma;

// Textures.
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform float shininess;

// Lighting.
uniform DirLight dirLight;
uniform PointLight pointLight;

// Camera.
uniform vec3 cameraPos;

// Shadows.
uniform sampler2D shadowMap;
uniform float minBias;
uniform bool debugNormalMap;
uniform bool debugVertexNormals;

vec3 GetNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, texCoords).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(worldPos);
    vec3 Q2 = dFdy(worldPos);
    vec2 st1 = dFdx(texCoords);
    vec2 st2 = dFdy(texCoords);

    vec3 N = normalize(normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

LightComponent CalcDirectionalLighting(DirLight light, vec3 normal, vec3 color, vec3 lightDir, vec3 halfwayDir)
{
    // Ambient.
    vec3 ambient = light.ambient * color;

    // normal.x * lightDir.x
    // normal.y * lightDir.y
    // normal.z * lightDir.z

    // Diffuse.
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * color;

    // Specular.
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = light.specular * spec * color;

    LightComponent result;
    result.ambient = ambient;
    result.diffuse = diffuse;
    result.specular = specular;

    return result;
}

vec3 CalcPointLighting(PointLight light, vec3 normal, vec3 color)
{
    // Calc light values.
    vec3 lightDir = normalize(light.position - worldPos);
    vec3 viewDir = normalize(cameraPos - worldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // Attenuation.
    float distance = length(light.position - worldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Ambient.
    vec3 ambient = light.ambient * color * attenuation;

    // Diffuse.
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * color * attenuation;

    // Specular.
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = light.specular * spec * color * attenuation;

    return (ambient + diffuse + specular);
}

// Returns 1.0 if this fragment is in shadow, otherwise 0.0.
float CalcShadow(vec3 normal, vec3 lightDir)
{
    // Perform perspective divide and transform to [0, 1].
    vec3 projCoords = ((fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5) + 0.5;

    // Sample depth from shadow map.
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // Get depth of current frag in world coords.
    float currentDepth = projCoords.z;

    // Apply bias to reduce shadow acne.
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), minBias);

    // Calculate if current frag is in shadow.
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    // Set lighting values.
    vec3 lightDir = normalize(-dirLight.direction);
    vec3 viewDir = normalize(cameraPos - worldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // Sample textures.
    vec3 albedo = texture(albedoMap, texCoords).rgb;
    vec3 norm = GetNormalFromMap();

    // Gamma correction from sRGB texture to linear color space for calculations.
    if (firstGamma)
    {
        float gamma = 2.2;
        albedo = pow(albedo, vec3(gamma));
    }

    // Calculate lighting.
    vec3 result = vec3(0.0, 0.0, 0.0);
    LightComponent directionalLighting = CalcDirectionalLighting(dirLight, norm, albedo, lightDir, halfwayDir);
    result += (directionalLighting.diffuse + directionalLighting.specular);
    result *= (1.0 - CalcShadow(normal, lightDir));
    result += directionalLighting.ambient;
    result += CalcPointLighting(pointLight, norm, albedo);

    // Output bright areas for blur/bloom shader.
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > bloomBrightness)
    {
        brightColor = vec4(result, 1.0);
    }
    else
    {
        brightColor = vec4(0.0, 0.0, 0.0, 0.0);
    }

    // Output Phong lighting.
    if (!debugNormalMap && !debugVertexNormals)
    {
        fragColor = vec4(result, 1.0);
    }
    else if (debugNormalMap && !debugVertexNormals)
    {
        fragColor = vec4(norm, 1.0);
    }
    else if (!debugNormalMap && debugVertexNormals)
    {
        fragColor = vec4(normal, 1.0);
    }
}