#version 330 core

layout (location = 0) in vec4 aPos;

//out vec2 TexCoords;

//uniform mat4 projection;
uniform mat4 model;

void main()
{
    //gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    //TexCoords = aPos.zw;

    gl_Position = projection * model * vec4(aPos.x, aPos.y, 0.0, 1.0);
}