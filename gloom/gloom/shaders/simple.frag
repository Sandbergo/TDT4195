#version 430 core

//in layout(location=0) flat vec4 colorV;

in layout(location=0) vec4 colorV;

out layout(location=0) vec4 color;

void main()
{
    color = colorV;
}
