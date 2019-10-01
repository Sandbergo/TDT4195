#version 430 core

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;
layout(location=2) in vec3 normal;

layout(location=1) out vec4 colorV;
layout(location=2) out vec3 normalV;

uniform mat4x4 viewMatrix;
uniform mat4x4 modelMatrix;

void main()
{
    colorV = color;
    normalV = normalize(mat3(modelMatrix) * normal);

    gl_Position = viewMatrix*modelMatrix*vec4(position.x, position.y, position.z, 1.0f);
}
