//Spotlight - Final light depends on two cut-off angles
//https://learnopengl.com/Lighting/Light-casters
//Input variables from CPU
attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_uv;

//Uniforms variables
uniform vec3 a_cameraposition;

uniform mat4 mvp;
uniform mat4 transform;
uniform mat4 a_normalMatrix;

//To Fragment Shader
varying vec3 v_point;
varying vec3 v_normal;
varying vec3 v_camerapos;
varying vec2 v_uv;

void main()
{
	vec4 v = vec4( a_position, 1.0 );

	vec4 v4_position = transform * v;
	v_point = v4_position.xyz;

	v_normal = normalize(mat3(a_normalMatrix)*normalize(a_normal));

	v_camerapos = a_cameraposition;

	v_uv = a_uv;

	gl_Position = mvp * transform * v;
}
