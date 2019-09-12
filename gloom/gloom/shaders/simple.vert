#version 430 core

in vec3 position;
in vec4 color;

out vec4 colorV;

void main()
{
    colorV = color;
    gl_Position = vec4(position, 1.0f);
}
