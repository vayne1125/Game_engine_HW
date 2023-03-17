//Fragment shader for per-pixel Phong shading
#version 430
out vec4 COLOR;
in  vec2 ST;
in  vec3 NORMAL;
in  vec3 GLOBAL; 

layout(location = 3) uniform sampler2D tex;
layout(location = 4) uniform vec4  dirLightPos;      //0: ¥­¦æ¥ú
layout(location = 5) uniform vec3  dirLightDir; 
layout(location = 6) uniform vec3  dirLightColor; 
layout(location = 7) uniform float  dirLightExpo;   //should up than 1 ,point should be 0
layout(location = 8) uniform float dirLightStr;

layout(location = 9) uniform float shine;
layout(location = 10) uniform vec3 cameraPos;
layout(location = 11) uniform float ambient;
layout(location = 12) uniform vec3 specular;

layout(location = 13) uniform vec4  spotLightPos; 
layout(location = 14) uniform vec3  spotLightDir; 
layout(location = 15) uniform vec3  spotLightColor; 
layout(location = 16) uniform float  spotLightExpo;   //should up to 1
layout(location = 17) uniform float spotLightStr;

void main()  {
	vec3 view = cameraPos - GLOBAL;
    vec4 tot = vec4(ambient, ambient, ambient,1.0f);
	
	vec3 L = (dirLightPos.xyz - GLOBAL)*dirLightPos.w + (dirLightPos.w - 1.0f)*dirLightDir;
	vec3 nL = normalize(L);
	vec3 ref = 2 * dot(NORMAL,nL) * NORMAL + nL;
	float lL = length(L);
	float att = dirLightStr / lL;
	float diffuse = clamp(dot(nL,NORMAL), 0.0f, 1.0f);
	float spe = clamp(pow(dot(ref, view),shine),0.0f,1.0f);
	float cone = dirLightExpo == 0 ? 1: pow(max(dot(nL,-normalize(dirLightDir)),0),dirLightExpo);

	vec3 L2 = (spotLightPos.xyz - GLOBAL)*spotLightPos.w + (spotLightPos.w - 1.0f)*spotLightDir;
	vec3 nL2 = normalize(L2);
	vec3 ref2 = 2 * dot(NORMAL,nL2) * NORMAL + nL2;
	float lL2 = length(L2);
	float att2 = spotLightStr / lL2;
	float diffuse2 = clamp(dot(nL2,NORMAL), 0.0f, 1.0f);
	float spe2 = clamp(pow(dot(ref2, view),shine),0.0f,1.0f);
	float cone2 = spotLightExpo == 0 ? 1: pow(max(dot(nL2,-normalize(spotLightDir)),0),spotLightExpo);

	tot += (att *(diffuse * vec4(dirLightColor,1.0f) + spe * vec4(specular, 1.0f))) * cone;
	
	tot += (att2 *(diffuse2 * vec4(spotLightColor,1.0f) + spe2 * vec4(specular, 1.0f))) * cone2;
	
    COLOR = texture(tex,ST)*tot; 
   //COLOR = texture(tex,ST); 

}
