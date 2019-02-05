#include "Texture.h"
#include "extra/ImageLoader.h"
#include "Utils.h"
#include <memory>

Texture::Texture( void )
    : m_GLTexture( -1 )
{

}

Texture::Texture( const std::string& p_oFileName )
	: m_GLTexture( -1 )
{
	load( p_oFileName );
}

Texture::~Texture( void )
{
    releaseTexture();
}

void Texture::releaseTexture( void )
{
    if( m_GLTexture > -1 )
    {
        glDeleteTextures( 1, &m_GLTexture );
        glFinish();
    }
}

void Texture::load( const std::string& p_oFileName )
{
	// EXERCISE PRACT 3
	std::unique_ptr<Image>pImage(loadBMP(addCurrentPath(p_oFileName).c_str())); // load image
	releaseTexture();

	// EXERCISE PRACT 3
	glGenTextures(1, &m_GLTexture);
	glBindTexture(GL_TEXTURE_2D, m_GLTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pImage->width, pImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	

}

