#version 430 core

//in layout(location=0) flat vec4 colorV;

layout(location=1) in vec4 colorIn;
layout(location=2) in vec3 normalIn;

layout(location=1) out vec4 colorOut;


void main()
{
   //colorOut = colorIn;
   colorOut = vec4(abs(normalIn.x), abs(normalIn.y), abs(normalIn.z), 1.0f);

}