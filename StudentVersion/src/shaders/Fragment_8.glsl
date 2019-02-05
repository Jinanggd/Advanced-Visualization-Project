
//Light struct
struct LightParameters{
	vec3 Position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};

//Material struct
struct MaterialParameters{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};

//Variables obtained from the CPU
uniform LightParameters light1;
uniform MaterialParameters material1;
uniform sampler2D u_textureimg;
uniform sampler2D u_texturenormal;
uniform float f_angleInner;
uniform float f_angleOutter;
uniform vec3 v3_spotdir;

//Variables from Vertex shader
varying vec3 v_point;
varying vec3 v_camerapos;
varying vec2 v_uv;
varying mat3 v_TBN;


vec4 computeAmbient(){

	vec3 Ca = vec3(light1.La * material1.Ka);
	vec4 ambient = vec4(Ca,1.0);
	return ambient;
}

vec4 computeDiffuse(vec3 L, vec3 N){

	float f_max = max(dot(L,N),0.0);
	vec3 Cd = vec3(light1.Ld * material1.Kd);
	vec4 diffuse = vec4(f_max*Cd,1.0);
	return diffuse;
}

vec4 computeSpecular(vec3 N, vec3 R, vec3 V){

	float f_pow = pow(max(dot(R,V),0.0),material1.Shininess);
	vec3 Cs = vec3(light1.Ls*material1.Ks);
	vec4 specular = vec4(f_pow*Cs,1.0);
	return specular;
}

float computeSpotlight(){

	vec3 norm_spotDir = normalize(v3_spotdir);
	vec3 norm_L = normalize(light1.Position-v_point);
	float f_angle = dot(norm_L,norm_spotDir);
	float intensity = clamp((f_angle-f_angleOutter)/(f_angleInner-f_angleOutter),0.0,1.0);

	return intensity;
	
}

void main()
{	
	vec3 texturenormal = texture(u_texturenormal,v_uv).rgb;
	texturenormal = normalize(texturenormal*2.0-1.0);
	texturenormal = normalize(v_TBN*texturenormal);
	vec4 texturecolor = texture(u_textureimg,v_uv);

	// L is the vector from point to the light, N the normalized normal, R reflected L with N, V is the vector from point to the camera
	vec3 vector_L = normalize(light1.Position-v_point);
	vec3 vector_N = texturenormal;
	vec3 vector_R = normalize(reflect(-vector_L,vector_N));
	vec3 vector_V = normalize(v_camerapos-v_point);

	float intensity = computeSpotlight();

	vec4 diffuse = computeDiffuse(vector_L,vector_N);
	vec4 ambient = computeAmbient();
	vec4 specular = computeSpecular(vector_N,vector_R,vector_V);

	vec4 color = ((ambient+diffuse)*texturecolor+specular)*intensity;
	color.w = 1.0;
	gl_FragColor = color; 
}
