//Input Values that is given in the host code
attribute vec3 Position;
attribute vec3 Normal;

//To Fragment Shader
varying vec3 out_normal;

//Constants
uniform mat4 mvp;
uniform mat4 transform;

void main()
{
	float maxcoord;
	float mincoord;

	maxcoord = max(Normal.x,Normal.y);
	maxcoord = max(maxcoord,Normal.z);

	mincoord = min(Normal.x,Normal.y);
	mincoord = min(mincoord,Normal.z);

	vec3 normal_eq = Normal;
	normal_eq.x = (normal_eq.x-mincoord)/(maxcoord-mincoord);
	normal_eq.y = (normal_eq.y-mincoord)/(maxcoord-mincoord);
	normal_eq.z = (normal_eq.z-mincoord)/(maxcoord-mincoord);

	out_normal = normal_eq;

	vec4 v = vec4( Position, 1.0 );
	gl_Position = mvp*transform*v;
}

























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































