#ifndef _TEXTURE_
#define _TEXTURE_

#include <string>
#include "includes.h"

class Texture
{
public:
	friend class Shader;

    Texture( void );
	Texture( const std::string& p_oFileName );
    ~Texture( void );

    void load( const std::string& p_oFileName );

private:
    void releaseTexture( void );

    GLuint m_GLTexture;
};

#endif
