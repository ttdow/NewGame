#version 330 core

out vec4 fragColor;

in vec3 normal;
in vec2 texCoords;

uniform vec3 color;
uniform vec3 cameraPos;

void main()
{
    fragColor = vec4(color, 1.0);
}