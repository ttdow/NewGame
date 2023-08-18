#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 boneWeights;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 totalPosition = vec4(0.0);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (boneIds[i] == -1)
        {
            continue;
        }

        if (boneIds[i] >= MAX_BONES)
        {
            totalPosition = vec4(aPos, 1.0);
            break;
        }

        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos, 1.0);
        totalPosition += localPosition * boneWeights[i];
    }

    normal = mat3(transpose(inverse(model))) * aNormal;
    fragPos = vec3(model * totalPosition);
    texCoords = aTexCoords;
    fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);

    gl_Position = projection * view * model * totalPosition;
}