#version 430 core

//in layout(location=0) flat vec4 colorV;

layout(location=1) in vec4 colorV;
layout(location=2) in vec3 normalV;

//layout(location=1) out vec4 colorOut;
out vec4 colorOut;


void main()
{
   vec3 lightDirection = normalize(vec3(0.8, -0.5,0.6));
   vec3 color = vec3(colorV.x, colorV.y, colorV.z)*max(0,dot(normalV, -lightDirection));
   colorOut = vec4(color, 1.0f);
   //colorOut = vec4(normalV.x, normalV.y, normalV.z, 1.0f);
}