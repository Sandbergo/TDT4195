#version 430 core

layout(location=0) in vec3 position;
layout(location=1) in vec4 colorIn;
layout(location=2) in vec3 normalIn;

layout(location=1) out vec4 colorOut;
layout(location=2) out vec3 normalOut;

uniform mat4x4 MVPmatrix;
uniform float scaler;

void main()
{
    colorOut = colorIn;
    normalOut = normalIn;

    gl_Position = MVPmatrix * vec4(position.x, position.y, position.z, 1.0f);
}
