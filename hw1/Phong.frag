//Fragment shader for per-pixel Phong shading
#version 430
out vec4 COLOR;
in  vec2 ST;
in  vec3 NORMAL;
in  vec3 GLOBAL; 

layout(location = 3) uniform sampler2D tex;
layout(location = 4) uniform vec4  lightInfopos; //0: ¥­¦æ¥ú
layout(location = 5) uniform vec3  lightInfodir; 
layout(location = 6) uniform vec3  lightInfocolor; 
layout(location = 7) uniform float  lightInfoexpo;   //should up to 1
layout(location = 8) uniform float lightInfostrength;
layout(location = 9) uniform float shine;
layout(location = 10) uniform vec3 cameraPos;
layout(location = 11) uniform float ambient;
layout(location = 12) uniform vec3 specular;

void main()  {
	vec3 view = cameraPos - GLOBAL;
    vec4 tot = vec4(ambient, ambient, ambient,1.0f);
	vec3 L = (lightInfopos.xyz - GLOBAL)*lightInfopos.w + (lightInfopos.w - 1.0f)*lightInfodir;
	vec3 nL = normalize(L);
	vec3 ref = 2 * dot(NORMAL,nL) * NORMAL + nL;
	float lL = length(L);
	float att = lightInfostrength / lL;
	float diffuse = clamp(dot(nL,NORMAL), 0.0f, 1.0f);
	float spe = clamp(pow(dot(ref, view),shine),0.0f,1.0f);
	float cone = lightInfoexpo == 0 ? 1: pow(max(dot(nL,-normalize(lightInfodir)),0),lightInfoexpo);
	tot += (att *(diffuse * vec4(lightInfocolor,1.0f) + spe * vec4(specular, 1.0f))) * cone;
	
   //COLOR = texture(tex,ST)*tot; 
   COLOR = texture(tex,ST); 

}
