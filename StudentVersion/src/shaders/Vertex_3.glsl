//Input value that is given by the host code
attribute vec3 Position;
attribute vec2 UVCoord;

//To Fragment Shader
varying vec2 TextureCoord;

//Constants
uniform mat4 mvp;
uniform mat4 transform;

void main()
{
	TextureCoord = UVCoord;

	//vec4 v = vec4( Position, 1.0 );
	//Diferencia entre gl_Position = mvp*transform*v amb gl_Position = mvp*gl_Vertex ???
	vec4 v= vec4(Position,1.0);
	gl_Position = mvp*transform*v;

}

