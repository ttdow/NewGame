#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 boneWeights;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 rotationMatrices[56];

void main()
{
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;
    texCoords = aTexCoords;

    gl_Position = projection * view * vec4(fragPos, 1.0);
}