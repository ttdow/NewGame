#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aTranslation;

out vec2 texCoord;
out vec4 fragPosLightSpace;
out vec3 normal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	texCoord = aTexCoord;

	// Calculate the camera's right and up vectors.
	vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
	vec3 up = vec3(view[0][1], view[1][1], view[2][1]);

	// Calculate a billboard transform so the grass always faces the camera.
	mat4 billboardTransform = mat4(vec4(right, 0.0), vec4(up, 0.0), vec4(cross(up, right), 0.0), vec4(aTranslation, 1.0));
	billboardTransform[0][0] *= 2.0;
	billboardTransform[1][1] *= 2.0;
	billboardTransform[2][2] *= 2.0;

	mat4 model = mat4(1.0);
	model[3][0] = aTranslation.x;
	model[3][1] = aTranslation.y;
	model[3][2] = aTranslation.z;

	vec4 fragPos = model * vec4(aPos, 1.0);
	fragPosLightSpace = lightSpaceMatrix * fragPos;

	normal = mat3(billboardTransform) * vec3(0.0, 1.0, 0.0);

	gl_Position = projection * view * billboardTransform * vec4(aPos, 1.0);
}