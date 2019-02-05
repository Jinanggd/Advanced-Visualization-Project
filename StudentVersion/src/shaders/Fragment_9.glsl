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
uniform LightParameters multiplelight[2];
uniform MaterialParameters material1;
uniform sampler2D u_textureimg;

//Variables from Vertex shader
varying vec3 v_point;
varying vec3 v_normal;
varying vec3 v_camerapos;
varying vec2 v_uv;

vec4 computeAmbient(int index){

	vec3 Ca = vec3(multiplelight[index].La * material1.Ka);
	vec4 ambient = vec4(Ca,1.0);
	return ambient;
}

vec4 computeDiffuse(vec3 L, vec3 N, int index){

	float f_max = max(dot(L,N),0.0);
	vec3 Cd = vec3(multiplelight[index].Ld * material1.Kd);
	vec4 diffuse = vec4(f_max*Cd,1.0);
	return diffuse;
}

vec4 computeSpecular(vec3 N, vec3 R, vec3 V, int index){

	float f_pow = pow(max(dot(R,V),0.0),material1.Shininess);
	vec3 Cs = vec3(multiplelight[index].Ls*material1.Ks);
	vec4 specular = vec4(f_pow*Cs,1.0);
	return specular;
}

vec4 computeColor(int index){

	vec4 texturecolor = texture(u_textureimg,v_uv);

	// L is the vector from point to the light, N the normalized normal, R reflected L with N, V is the vector from point to the camera
	vec3 vector_L = normalize(multiplelight[index].Position-v_point);
	vec3 vector_N = v_normal;
	vec3 vector_R = normalize(reflect(-vector_L,vector_N));
	vec3 vector_V = normalize(v_camerapos-v_point);

	vec4 diffuse = computeDiffuse(vector_L,vector_N, index);
	vec4 ambient = computeAmbient(index);
	vec4 specular = computeSpecular(vector_N,vector_R,vector_V, index);

	vec4 color = (ambient+diffuse)*texturecolor+specular;
	color.w = 1.0;
	return color; 
}

void main()
{	
	vec4 finalcolor;
	for(int i=0;i<2;i++){
		finalcolor += computeColor(i);
	}
	finalcolor.w = 1.0f;
	gl_FragColor = finalcolor;
}
