//Toon or Cel Shading
attribute vec3 a_position;
attribute vec3 a_normal;


uniform vec3 a_cameraposition;

uniform mat4 mvp;
uniform mat4 transform;
uniform mat4 a_normalMatrix;
uniform vec3 v3_color;

varying vec3 v_color;
varying vec3 v_point;
varying vec3 v_normal;
varying vec3 v_camerapos;

void main()
{
	vec4 v = vec4( a_position, 1.0 );

	vec4 v4_position = transform * v;
	v_point = v4_position.xyz;

	v_normal = normalize(mat3(a_normalMatrix)*normalize(a_normal));

	v_camerapos = a_cameraposition;
	v_color = v3_color;
	gl_Position = mvp * transform * v;
}
