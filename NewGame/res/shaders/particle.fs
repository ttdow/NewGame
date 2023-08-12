#version 330 core

in vec2 texCoords;
in vec4 particleColor;

out vec4 fragColor;

uniform sampler2D texture;
uniform vec4 color;

void main()
{
    fragColor = color;
}