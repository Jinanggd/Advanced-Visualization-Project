//Toon or Cel Shading
//Light struct
struct LightParameters{
	vec3 Position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};

//Material struct
//struct MaterialParameters{
//	vec3 Ka;
//	vec3 Kd;
//	vec3 Ks;
//	float Shininess;
//};

//Variables obtained from the CPU
uniform LightParameters cartoon;
//uniform MaterialParameters material1;

//Variables from Vertex shader
varying vec3 v_point;
varying vec3 v_normal;
varying vec3 v_camerapos;
varying vec3 v_color;

vec4 computeColor(float intensity){
	vec3 color;

	if(intensity > 0.99)
		color = vec3(1,1,1);
	else if(intensity > 0.95)
		color = (v_color*0.9);
	else if (intensity > 0.5)
		color = (v_color*0.5);
	else if(intensity > 0.25)
		color = (v_color*0.3);
	else if(intensity >0.1)
		color = (v_color*0.1);
	else 
		color = vec3(0,0,0);
	return vec4(color,1.0);
}
void main()
{	
	
	// L is the vector from point to the light, N the normalized normal, R reflected L with N, V is the vector from point to the camera
	vec3 vector_L = normalize(cartoon.Position-v_point);
	vec3 vector_N = v_normal;
	vec3 vector_V = normalize(v_camerapos-v_point);

	float intensity = dot(vector_L,vector_N);
	
	gl_FragColor = computeColor(intensity); 
}
