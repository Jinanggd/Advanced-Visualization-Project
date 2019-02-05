attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_uv;
attribute vec3 a_tangent;
attribute vec3 a_bitangent;


uniform vec3 a_cameraposition;

uniform mat4 mvp;
uniform mat4 transform;
uniform mat4 a_normalMatrix;

varying vec3 v_point;
varying vec3 v_camerapos;
varying vec2 v_uv;
varying mat3 v_TBN;

void main()
{
	vec4 v = vec4( a_position, 1.0 );

	vec4 v4_position =  transform * v;
	v_point = v4_position.xyz;

	vec3 v_normal = mat3(transform)*normalize(a_normal);
	vec3 v_tangent =mat3(transform)*normalize(a_tangent);
	vec3 v_bitangent = mat3(transform)*normalize(a_bitangent);

	mat3 TBN = mat3(
		v_tangent,
		v_bitangent,
		v_normal
	);

	v_TBN = TBN;

	v_camerapos = a_cameraposition;

	v_uv = a_uv;

	gl_Position = mvp * transform * v;
}
