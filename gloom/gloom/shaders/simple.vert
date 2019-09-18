#version 430 core

in vec3 position;
in vec4 color;

out layout(location=0) vec4 colorV;

uniform mat4x4 MVPmatrix;
uniform float scaler;

void main()
{
    mat4x4 matrix = mat4(1);
    //a 
    //matrix [0][0] = scaler;
    //b
    //matrix [0][1] = scaler; 
    //c
    //matrix [0][3] = scaler;
    //d
    //matrix [1][0] = scaler;
    //e
    //matrix [1][1] = scaler;
    //f
    //matrix [1][3] = scaler;

    colorV = color;
    //gl_Position = matrix * vec4(position.x, position.y, position.z, 1.0f);
    gl_Position = MVPmatrix * vec4(position.x, position.y, position.z, 1.0f);
}
