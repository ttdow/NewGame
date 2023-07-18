#version 330 core

out vec4 fragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    fragColor = vec4(1.0); // set all 4 vector values to 1.0
}