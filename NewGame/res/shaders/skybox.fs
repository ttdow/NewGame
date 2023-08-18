#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    vec4 color = texture(skybox, TexCoords);

    FragColor = color - vec4(0.4, 0.5, 0.4, 0.0);
}