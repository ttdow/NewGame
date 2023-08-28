#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

in vec2 texCoords;
in vec3 normal;
in vec3 worldPos;
in vec4 fragPosLightSpace;

// Textures.
uniform sampler2D albedoMap;
uniform sampler2D normalMap;

// Camera.
uniform vec3 cameraPos;

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

void main()
{
    // Sample textures.
    vec3 albedo = texture(albedoMap, texCoords).rgb;
    vec3 norm = GetNormalFromMap();
    
    // Set geometry buffers.
    gPosition = worldPos;
    gNormal = norm;
    gAlbedo = vec4(albedo, 1.0);
}