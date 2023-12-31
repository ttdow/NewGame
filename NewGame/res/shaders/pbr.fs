#version 330 core

out vec4 fragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in vec4 fragPosLightSpace;

// Material textures.
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// Lights.
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 lightDir;

// Shadows.
uniform sampler2D shadowMap;
uniform float minBias;

// Camera.
uniform vec3 cameraPos;

const float PI = 3.14159265359;

vec3 GetNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(WorldPos);
    vec3 Q2 = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N = normalize(Normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
    // Sample textures.
    vec3 albedo = texture(albedoMap, TexCoords).rgb;
    float metallic = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;
    float ao = texture(aoMap, TexCoords).r;

    // Convert from sRGB space to linear color space.
    float gamma = 2.2;
    albedo = pow(albedo, vec3(gamma));

    // Calculate normal from normal map.
    vec3 N = GetNormalFromMap();

    // Calculate view direction from fragment position and camera position.
    vec3 V = normalize(cameraPos - WorldPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // Reflectance equation.
    vec3 Lo = vec3(0.0);
    //for (int i = 0; i < 4; i++)
    //{
    // Calculate per-light radiance.
    //vec3 L = normalize(lightPositions[i] - WorldPos);
    vec3 L = normalize(-lightDir);
    vec3 H = normalize(V + L);
    //float distance = length(lightPositions[i] - WorldPos);
    //float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightColors[0]; //[i] * attenuation;
        
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    // Add to outgoing radiance Lo.
    float NdotL = max(dot(N, L), 0.0);
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    //}

    vec3 ambient = vec3(0.03) * albedo; // * ao;

    // Shadows
    vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w * 0.5) + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(N, lightDir)), minBias);
    float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;

    //if (projCoords.z > 1.0)
    //{
    //       shadow = 0.0;
    //}

    vec3 color = ambient + (Lo * shadow);
    color = color / (color + vec3(1.0));

    // Convert from linear color space back to sRGB space.
    color = pow(color, vec3(1.0 / gamma));

    fragColor = vec4(color, 1.0);
}