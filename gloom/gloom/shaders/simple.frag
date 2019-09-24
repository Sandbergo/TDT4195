#version 430 core

//in layout(location=0) flat vec4 colorV;


in vec4 colorIn;
in vec3 normalIn;

out vec4 colorOut;


void main()
{
   //colorOut = colorIn;
   //colorOut = vec4(abs(normalIn.x), abs(normalIn.y), abs(normalIn.z), 1.0f);
   if (normalIn.x == 0 && normalIn.y == 0 && normalIn.z == 0){
       colorOut = vec4(0.0f, 1.0f, 0.0f, 1.0f);
   }
   else{
       colorOut = vec4(1.0f, 0.0f, 0.0f, 1.0f);
   }
}