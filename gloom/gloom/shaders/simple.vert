#version 430 core

in vec3 position;

void main()
{
    vec3 position_mirrored = vec3(-position.x, -position.y, position.y);
    gl_Position = vec4(position_mirrored, 1.0f);
}
