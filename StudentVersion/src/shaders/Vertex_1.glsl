attribute vec3 Position;

uniform mat4 mvp;
uniform mat4 transform;

void main()
{
	vec4 v = vec4( Position, 1.0 );
	gl_Position = mvp * transform * v;
}
