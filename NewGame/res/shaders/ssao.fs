#version 330 core

out vec4 fragColor;

in vec2 texCoords;

// Textures.
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

// Kernel samples.
uniform vec3 samples[64];

// Camera.
uniform mat4 projection;
uniform mat4 view;
uniform vec3 cameraPos;

// Window.
uniform float windowWidth;
uniform float windowHeight;

// Tile noise texture over screen.
//const 
void main()
{
    // SSAO parameters.
    int kernelSize = 64;
    float radius = 0.5;
    float bias = 0.025;

    vec2 noiseScale = vec2(windowWidth / 4.0, windowHeight / 4.0);
    
    vec3 worldPos = texture(gPosition, texCoords).xyz;
    vec3 normal = texture(gNormal, texCoords).xyz;
    vec3 randomVec = texture(texNoise, texCoords * noiseScale).xyz;

    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; i++)
    {
        vec3 samplePos = TBN * samples[i];
        samplePos = worldPos + samplePos * radius;

        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = texture(gPosition, offset.xy).z;

        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(worldPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);

    fragColor = vec4(vec3(occlusion), 1.0);
}