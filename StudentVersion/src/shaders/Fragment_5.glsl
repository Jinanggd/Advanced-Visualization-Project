//Point Light - computed using light and vertex position
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
uniform LightParameters point_light;
uniform MaterialParameters material1;
uniform sampler2D u_textureimg;

//Variables from Vertex shader
varying vec3 v_point;
varying vec3 v_normal;
varying vec3 v_camerapos;
varying vec2 v_uv;



vec4 computeAmbient(){

	vec3 Ca = vec3(point_light.La * material1.Ka);
	vec4 ambient = vec4(Ca,1.0);
	return ambient;
}

vec4 computeDiffuse(vec3 L, vec3 N){

	float f_max = max(dot(L,N),0.0);
	vec3 Cd = vec3(point_light.Ld * material1.Kd);
	vec4 diffuse = vec4(f_max*Cd,1.0);
	return diffuse;
}

vec4 computeSpecular(vec3 N, vec3 R, vec3 V){

	float f_pow = pow(max(dot(R,V),0.0),material1.Shininess);
	vec3 Cs = vec3(point_light.Ls*material1.Ks);
	vec4 specular = vec4(f_pow*Cs,1.0);
	return specular;
}

void main()
{	
	// L is the vector from point to the light, N the normalized normal, R reflected L with N, V is the vector from point to the camera
	vec3 vector_L = normalize(point_light.Position-v_point);
	vec3 vector_N = v_normal;
	vec3 vector_R = normalize(reflect(-vector_L,vector_N));
	vec3 vector_V = normalize(v_camerapos-v_point);

	vec4 diffuse = computeDiffuse(vector_L,vector_N);
	vec4 ambient = computeAmbient();
	vec4 specular = computeSpecular(vector_N,vector_R,vector_V);

	vec4 color = (ambient + diffuse)*texture(u_textureimg,v_uv) + specular;
	color.w = 1.0;
	gl_FragColor = color; 
}
