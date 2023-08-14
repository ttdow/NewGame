#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texture;
uniform vec4 color;

void main()
{
    fragColor = color;
}