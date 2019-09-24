#version 430 core

in layout(location=0) vec3 position;
in layout(location=1) vec4 colorIn;
in layout(location=2) vec3 normalIn;

out layout(location=1) vec4 colorOut;
out vec3 normalOut;



uniform mat4x4 MVPmatrix;
uniform float scaler;

void main()
{
    mat4x4 matrix = mat4(1);

    colorOut = colorIn;
    normalOut = normalIn;

    gl_Position = MVPmatrix * vec4(position.x, position.y, position.z, 1.0f);
}
