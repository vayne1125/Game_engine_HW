//Vertex shader for per-vertex Phong shading
/*
varying vec3 N; //normal vector
varying vec3 L; //light direction vector
varying vec3 E; //V vector for specular.
*/
#version 430
layout(location = 0) in vec3 vertexCor; //index
layout(location = 1) in vec3 textureCor; //index
layout(location = 2) in vec3 normalCor; //index
layout(location = 0) uniform mat4 view;
layout(location = 1) uniform mat4 projection;
layout(location = 2) uniform mat4 model;
void main()
{
   
   /*
   vec4 eyePosition = gl_ModelViewMatrix*gl_Vertex;
   
   vec4 eyeLightPos = gl_ModelViewMatrix*gl_LightSource[0].position;

  //Compute the key vector parameters for the vertex.
   N =  normalize(gl_NormalMatrix*gl_Normal);
   L = eyeLightPos.xyz - eyePosition.xyz ;
   E = -eyePosition.xyz;
   */
   
   gl_Position = projection*view*model*vec4(vertexCor,1);
}