/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include <memory>
#include "Light.h"
#include "Material.h"

class Application
{
public:
	static Application* instance;
	
	bool b_renderLight;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;
	int shaderMode;

	float time;

	//keyboard state
	const Uint8* keystate;

	// EXERCISE PRACT 2
	enum class PoligonMode
	{
		POINT = GL_POINTS,
		LINE = GL_LINES,
		FILL = GL_TRIANGLES
	};

	//Defines the poligon mode
	PoligonMode poligonMode;

	std::vector<std::unique_ptr<Mesh>> aseFiles;
	int aseIndex;
	// EXERCISE PRACT 2

	// EXERCISE PRACT 3
	std::vector<std::unique_ptr<Texture>> aseTextures;

	//Shaders
	Shader shader1;
	Shader shader2;
	Shader shader3;
	Shader shader4;
	Shader shader5;
	Shader shader6;
	Shader shader7;
	Shader shader8;
	Shader shader9;
	//Light
	Light* light;
	Material* material;
	std::vector<Light*> v_lights;

	int shaderId;
	// EXERCISE PRACT 3

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame

	Camera* camera; //our global camera

	Application( SDL_Window* window );
	~Application( void );

	void init( void );
	void createMultipleLights();

	void render( void );
	void update( float dt );

	void SwitchingShader(const int aseIndex,const Matrix44& transform);
	// EXERCISE PRACT 3
	
	void directionalLight(Matrix44 transformPlanet, Matrix44 transformMOON, Matrix44 transformASTEROID, Matrix44 rotationPlanet,
		Matrix44 rotationSun, Matrix44 rotationAst, Vector3 axisRotation);
	void pointLight(Matrix44 transformPlanet, Matrix44 transformMOON, Matrix44 transformASTEROID, Matrix44 rotationPlanet,
		Matrix44 rotationSun, Matrix44 rotationAst, Vector3 axisRotation);
	void spotLight(Matrix44 transformPlanet, Matrix44 transformMOON, Matrix44 transformASTEROID, Matrix44 rotationPlanet,
		Matrix44 rotationSun, Matrix44 rotationAst, Vector3 axisRotation);
	void cartoon(Matrix44 transformPlanet, Matrix44 transformMOON, Matrix44 transformASTEROID, Matrix44 rotationPlanet,
		Matrix44 rotationSun, Matrix44 rotationAst, Vector3 axisRotation);
	void multiplelights(Matrix44 transformPlanet, Matrix44 transformMOON, Matrix44 transformASTEROID, Matrix44 rotationPlanet,
		Matrix44 rotationSun, Matrix44 rotationAst, Vector3 axisRotation);
	void normalMapping();

	void renderShader1(const int aseIndex, const Matrix44& transform );
	void renderShader2(const int aseIndex, const Matrix44& transform);
	void renderShader3(const int aseIndex, const Matrix44& transform);
	void renderShader4(const int aseIndex, const Matrix44& transform);
	void renderShader5(const int aseIndex, const Matrix44& transform);
	void renderShader6(const int aseIndex, const Matrix44& transform, float f_inagle, float f_outangle);
	void renderShader7(const int aseIndex, const Matrix44& transform, Vector3 basecolor);
	void renderShader8(const int aseIndex, const Matrix44& transform);
	void renderShader9(const int aseIndex, const Matrix44& transform);

	void renderBackgroud();

	// EXERCISE PRACT 3

	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButton( SDL_MouseButtonEvent event );
	void onResize( SDL_Event e );

	void setWindowSize( int width, int height );
};


#endif 