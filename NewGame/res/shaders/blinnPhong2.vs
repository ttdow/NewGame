#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoords;
out vec3 normal;
out vec3 viewNormal;
out vec3 worldPos;
out vec3 viewPos;
out vec4 fragPosLightSpace;

uniform bool invertedNormals;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
	texCoords = aTexCoords;
	normal = aNormal;

	mat3 normalMatrix = transpose(inverse(mat3(view * model)));
	viewNormal = normalMatrix * (invertedNormals ? -aNormal : aNormal);

	worldPos = vec3(model * vec4(aPos, 1.0));
	viewPos = vec3(view * model * vec4(aPos, 1.0));
    fragPosLightSpace = lightSpaceMatrix * vec4(worldPos, 1.0);

	gl_Position = projection * view * vec4(worldPos, 1.0);
}