//Vertex shader for per-vertex Phong shading
#version 450
layout(location = 0) in vec3 vertexCor; //index
layout(location = 1) in vec2 textureCor; //index
layout(location = 2) in vec3 normalCor; //index

layout(location = 0) uniform mat4 view;
layout(location = 1) uniform mat4 projection;
layout(location = 2) uniform mat4 model;

out vec2 ST;
out vec3 NORMAL;
out vec3 GLOBAL;
void main()
{
   GLOBAL = (model*vec4(vertexCor,1)).xyz;
   ST = textureCor;
   NORMAL = normalize((model*vec4(normalize(normalCor),0)).xyz);

   gl_Position = projection*view*model*vec4(vertexCor,1);
}