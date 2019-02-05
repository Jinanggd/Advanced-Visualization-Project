varying vec2 TextureCoord;

uniform sampler2D TextureImg;

void main()
{	
	gl_FragColor = texture(TextureImg,TextureCoord);
}
