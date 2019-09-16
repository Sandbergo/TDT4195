#version 430 core

in vec3 position;
in vec4 color;

out vec4 colorV;

uniform mat4 MVPmatrix;

void main()
{
    mat4x4 matrix = mat4(1);
    matrix [1][0] = -0.5;

    colorV = color;
    gl_Position = MVPmatrix * vec4(position.x, position.y, position.z, 1.0f);
}
