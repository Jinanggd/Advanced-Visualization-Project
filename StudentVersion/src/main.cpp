/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com

	MAIN:
	 + This file creates the window and the g_oApplication instance.
	 + It also contains the mainloop
	 + This is the lowest level, here we access the system to create the opengl Context
	 + It takes all the events from SDL and redirect them to the g_oApplication
*/

//ORIOL CERVELLO 182542
//CAROLINA DIAS 182525
//LIE JIN 182517

#include "includes.h"

#include "Maths.h"
#include "Mesh.h"
#include "Camera.h"
#include "Utils.h"
#include "Application.h"
//#include <GL/glut.h>

#include <iostream> //to output
#include <memory>

long last_time = 0; //this is used to calcule the elapsed time between frames

std::unique_ptr<Application> g_oApplication;

//Drawing string does not work since we cannot include GL/glut.h libraries
//void *currentfont;
//
//void setFont(void *font) {
//	currentfont = font;
//}
//
//void drawstring(float x, float y, float z, char* string) {
//	char* c;
//	glRasterPos3f(x, y, z);
//	for (c = string; *c != '\0'; c++) {
//		glColor3f(0.0, 0.0, 0.0);
//		glutBitmapCharacter(currentfont, *c);
//	}
//}
//
//void menuText(void) {
//	setFont(GLUT_BITMAP_HELVETICA_10);
//	glColor3f(0.0, 1.0, 1.0);
//	drawstring(150.0, 455.0, 1.0, "DRAAAAAAAAAAAAAAAAAAAAAAAAAAW");
//}

//DEPRACATED
//void PrintText() {
//	std::string separator = "/********************************************************************/";
//	std::cout << "/************************* PRACTISE 2 MENU ************************/" << std::endl;
//	std::cout << "/* Camera Movement */" << std::endl;
//	std::cout << "+ Press 'W' to move forward" << std::endl;
//	std::cout << "+ Press 'S' to move backward" << std::endl;
//	std::cout << "+ Press 'D' to move to the right" << std::endl;
//	std::cout << "+ Press 'A' to move to the left" << std::endl;
//	std::cout << "+ Click and Drag with Left button of the Mouse to rotate" << std::endl;
//	std::cout << separator << std::endl;
//	std::cout << "/* Model Selection */" << std::endl;
//	std::cout << "You can change the current MODELS in ASE with '-' and '+' buttons" << std::endl;
//	std::cout << "+ Press 'UP' Arrow to rotate up 90º the current model " << std::endl;
//	std::cout << "+ Press 'DOWN' Arrow to rotate down 90º the current model" << std::endl;
//	std::cout << "+ Press 'RIGHT' Arrow to rotate right 90º the current model" << std::endl;
//	std::cout << "+ Press 'Left' Arrow to rotate left 90º the current model" << std::endl;
//	std::cout << separator << std::endl;
//	std::cout << "/* Primitive Selection */" << std::endl;
//	std::cout << "+ Press F to use GL_TRIANGLES primitive" << std::endl;
//	std::cout << "+ Press P to use GL_POINTS primitive" << std::endl;
//	std::cout << "+ Press L to use GL_LINES primitive" << std::endl;
//	std::cout << separator << std::endl;
//
//}
// *********************************
//create a window using SDL
SDL_Window* createWindow( const char* caption, int width, int height, bool fullscreen = false )
{
	int multisample = 8;
	bool retina = true; //change this to use a retina display

	//set attributes
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 ); //or 24
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );

	//antialiasing (disable this lines if it goes too slow)
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, multisample ); //increase to have smoother polygons



	//create the window
	SDL_Window *window = SDL_CreateWindow(
		caption, 100, 100, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
		( retina ? SDL_WINDOW_ALLOW_HIGHDPI : 0 ) |
		( fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0 ) );
	if( !window )
	{
		fprintf( stderr, "Window creation error: %s\n", SDL_GetError() );
		exit( -1 );
	}

	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext( window );

	//in case of exit, call SDL_Quit()
	atexit( SDL_Quit );

	//get events from the queue of unprocessed events
	SDL_PumpEvents(); //without this line asserts could fail on windows

	//launch glew to extract the opengl extensions functions from the DLL
#ifdef USE_GLEW
	glewInit();
#endif

	return window;
}


//The application main loop
void mainLoop()
{
	SDL_Event sdlEvent;
	int x, y;

	/*menuText();*/
	// EXERCISE PRACT 1

	while( 1 )
	{

		//read keyboard state and stored in keystate
		g_oApplication->keystate = SDL_GetKeyboardState( NULL );

		//render frame
		g_oApplication->render();

		//update events
		while( SDL_PollEvent( &sdlEvent ) )
		{
			switch( sdlEvent.type )
			{
				case SDL_QUIT: return; break; //EVENT for when the user clicks the [x] in the corner
				case SDL_MOUSEBUTTONDOWN: //EXAMPLE OF sync mouse input
					g_oApplication->onMouseButton( sdlEvent.button );
					break;
				case SDL_MOUSEBUTTONUP:
					//...
					break;
				case SDL_KEYDOWN: //EXAMPLE OF sync keyboard input
					g_oApplication->onKeyPressed( sdlEvent.key );
					break;
				case SDL_WINDOWEVENT:
					switch( sdlEvent.window.event )
					{
						case SDL_WINDOWEVENT_RESIZED: //resize opengl context
							g_oApplication->setWindowSize( sdlEvent.window.data1, sdlEvent.window.data2 );
							break;
					}
			}
		}

		//get mouse position and delta (do after pump events)
		// EXERCISE PRACT 1
		
		g_oApplication->mouse_state = SDL_GetMouseState(&x, &y);
		

		g_oApplication->mouse_delta.x = g_oApplication->mouse_position.x - x;
		g_oApplication->mouse_delta.y = g_oApplication->mouse_position.y - y;

		g_oApplication->mouse_position.x = x;
		g_oApplication->mouse_position.y = y;
	
		//std::cout << " Previous x: " << x_ant << " Previous y: " << y_ant << " Current x"<< x<< "Current y"<< y <<std::endl;

		//update logic
		long now = SDL_GetTicks();
		float elapsed_time = float( now - last_time ) * 0.001f; //0.001 converts from milliseconds to seconds
		last_time = now;
		g_oApplication->time = float( now ) * 0.001f;
		g_oApplication->update( elapsed_time );

		//check errors in opengl only when working in debug
#ifdef _DEBUG
		checkGLErrors();
#endif
	}

	return;
}

int main( int argc, char **argv )
{
	std::cout << "Initiating g_oApplication..." << std::endl;

	//PrintText(); DEPRACATED --- Menu per la practica 2
	//prepare SDL
	SDL_Init( SDL_INIT_EVERYTHING );

	bool fullscreen = false; //change this to go fullscreen
	Vector2 size( 800, 600 );

	if( fullscreen )
		size = getDesktopSize( 0 );

	//create the g_oApplication window (WINDOW_WIDTH and WINDOW_HEIGHT are two macros defined in includes.h)
	SDL_Window* window = createWindow( "AV", int( size.x ), int( size.y ), fullscreen );
	if( !window )
		return 0;

	//std::cout << "Select either printing a Cube ( C ) or a Plane ( P ) " << std::endl;

	//glutKeyboardFunc(KeyBoardFunc);
	//launch the g_oApplication (g_oApplication is a global variable)
	g_oApplication.reset( new Application( window ) );
	g_oApplication->init();

	//main loop, application gets inside here till user closes it
	mainLoop();

	return 0;
}
