/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
    This file has all the includes so the app works in different systems.
    It is a little bit low level so do not worry about the code.
*/

#ifndef INCLUDES_H
#define INCLUDES_H

#ifndef APIENTRY
    #define APIENTRY
#endif

//#define USE_GLEW
#define GLEW_STATIC

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_opengl.h>
    #include <OpenGL/glu.h>
#else
    #define USE_GLEW
    #define GLEW_STATIC

    #ifdef WIN32
        #include <windows.h>
        #include "GL/glew.h"
        #include "GL/GLU.h"
        #include "SDL.h"
        #include "SDL_opengl.h"
		/*#include "GL/glut.h"*/

    #else
        #include <GL/glew.h>
        #include <SDL2/SDL.h>
        #include <SDL2/SDL_opengl.h>
        #include <GL/glu.h>
    #endif
#endif

#include <iostream>

//used to access opengl extensions
#define REGISTER_GLEXT(RET, FUNCNAME, ...) typedef RET ( * FUNCNAME ## _func)(__VA_ARGS__); FUNCNAME ## _func FUNCNAME = NULL;
#define IMPORT_GLEXT(FUNCNAME) FUNCNAME = (FUNCNAME ## _func) SDL_GL_GetProcAddress(#FUNCNAME); if (FUNCNAME == NULL) { std::cout << "ERROR: This Graphics card doesnt support " << #FUNCNAME << std::endl; }

//OPENGL EXTENSIONS

#endif
