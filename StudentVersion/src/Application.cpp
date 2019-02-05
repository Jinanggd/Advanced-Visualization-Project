#include "Application.h"
#include "Utils.h"
#include "Mesh.h"

#include <cmath>

#define PLANET 0
#define SUN 1
#define MOON 2
#define ASTEROID 3
#define BACK 4
#define LIGHT 5
#define TEST 6

//some globals
Mesh* mesh = NULL;

Application* Application::instance = NULL;

Application::Application( SDL_Window* window )
{
	this->window = window;
	instance = this;

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;

	keystate = NULL;
}

Application::~Application( void )
{
	delete camera;
	delete mesh;

	aseFiles.clear();
}

//Here we have already GL working, so we can create meshes and textures
void Application::init( void )
{
	b_renderLight = false;
	std::cout << " * Path: " << getPath() << std::endl;

	//OpenGL flags
	//glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
	//To have double sided polygons
	//glDisable( GL_CULL_FACE );

	//create our camera
	camera = new Camera();
	camera->lookAt( Vector3( 0, 500, 500 ), Vector3( 0, 0, 0 ), Vector3( 0, 1, 0 ) ); //position the camera and point to 0,0,0
	camera->setPerspective( 70, window_width / (float)window_height, 0.1f, 1000000000 ); //set the projection, we want to be perspective

	//LIGHTS
	light = new Light();
	createMultipleLights();

	//MATERIAL
	material = new Material();

	//Load models for the scene
	//aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("dark_fighter_6.ASE", 2.0f, false)));
	
	//aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("Squirrel.obj", 2.0f, true)));
	
	//aseFiles[0]->invertMe();
	//aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("esferaLie.obj", 1000.0f, true)));
	//aseFiles[0]->invertMe();
	aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("esferaLie.obj", 1.0f, true)));
	aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("esferaLie.obj", 1.5f, true)));
	aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("esferaLie.obj", 0.5f, true)));
	aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("Asteroid.obj", 0.8f, true)));
	aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("Box2.ASE", 50.0f, false)));
	aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("esferaLie.obj", 0.2f, true)));
	aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("tree.obj", 100.0f, true)));
	//aseFiles[BACK]->invertMe();
	//aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("tree.obj", 2.0f, true)));
	//aseFiles.push_back(std::unique_ptr<Mesh>(new Mesh("esferaLie.obj", 0.1f, true)));

	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("grass.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("sun.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("moon.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("Asteroid.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("galaxy.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("galaxy.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("tree.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("treenormal.bmp")));

	/*aseTextures.push_back(std::unique_ptr<Texture>(new Texture("tree.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>()*/
	shaderMode = 1;
	aseIndex = 0;
	// EXERCISE PRACT 2

	//SHADERS LOAD

	shader1.load("Vertex_1.glsl", "Fragment_1.glsl");
	shader2.load("Vertex_2.glsl", "Fragment_2.glsl");
	shader3.load("Vertex_3.glsl", "Fragment_3.glsl");
	shader4.load("Vertex_4.glsl", "Fragment_4.glsl");
	shader5.load("Vertex_5.glsl", "Fragment_5.glsl");
	shader6.load("Vertex_6.glsl", "Fragment_6.glsl");
	shader7.load("Vertex_7.glsl", "Fragment_7.glsl");
	shader8.load("Vertex_8.glsl", "Fragment_8.glsl");
	shader9.load("Vertex_9.glsl", "Fragment_9.glsl");

	poligonMode = PoligonMode::FILL;

	//hide the cursor
	SDL_ShowCursor( true ); //hide or show the mouse
}

void Application::createMultipleLights() {
	Light* l1 = new Light("multiplelight[0]", Vector3(-300, 300, 0), Vector3(0, 0, 1), Vector3(0, 0, 1), Vector3(0.4, 0.4, 0.4));
	//l1->redLight("multiplelight[0]");
	Light* l2 = new Light("multiplelight[1]", Vector3(0, 300, 300), Vector3(1, 1, 0), Vector3(1, 1, 0), Vector3(0.4, 0.4, 0.4));
	//l2->blueLight("multiplelight[1]");
	//Light* l3 = new Light("multiplelight[2]", Vector3(-300, 300, 0), Vector3(0,0,1), Vector3(0, 0, 1), Vector3(0.9, 0.9, 0.9));
	//l3->greenLight("multiplelight[2]");
	
	v_lights.push_back(l1);
	v_lights.push_back(l2);
}

//what to do when the image has to be draw
void Application::render( void )
{
	glClearColor( 1.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//set the clear color (the background color)
	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	// Clear the window and the depth buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Put the camera matrices on the stack of OpenGL (only for fixed rendering)
	camera->set();

	//Draw out world
	//drawGrid( 500 ); //background grid

	// EXERCISE PRACT 3
	glEnable( GL_BLEND ); // Enable alpha channel
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	Matrix44 transform; // Circular movement
	SwitchingShader(aseIndex, transform);

	//renderSpheres();
	// EXERCISE PRACT 3

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow( this->window );
}

void Application::directionalLight(Matrix44 transformPlanet, Matrix44 transformMOON, Matrix44 transformASTEROID, Matrix44 rotationPlanet,
	Matrix44 rotationSun, Matrix44 rotationAst, Vector3 axisRotation) {

	//Light and Material
	light->whiteLight("directional");
	material->pearl("material1");
	light->setPosition(Vector3(10000, 10000, 10000));
	//Renders
	//light->setPosition(transformMOON);
	renderShader4(SUN, rotationSun);
	renderShader4(MOON, transformMOON*rotationPlanet);
	renderShader4(ASTEROID, transformASTEROID*rotationAst);
	renderShader4(PLANET, transformPlanet*rotationPlanet);
}

void Application::pointLight(Matrix44 transformPlanet, Matrix44 transformMOON, Matrix44 transformASTEROID, Matrix44 rotationPlanet,
	Matrix44 rotationSun, Matrix44 rotationAst, Vector3 axisRotation) {

	//Light and Material
	
	light->whiteLight("point_light");
	material->pearl("material1");
	light->setPosition(Vector3(345.418f, 259.637f, 4.53244f));
	//light->setPosition(camera->eye);
	//Renders
	//light->setPosition(transformMOON);
	renderShader5(SUN, rotationSun);
	renderShader5(MOON, transformMOON*rotationPlanet);
	renderShader5(ASTEROID, transformASTEROID*rotationAst);
	renderShader5(PLANET, transformPlanet*rotationPlanet);
}

void Application::spotLight(Matrix44 transformPlanet, Matrix44 transformMOON, Matrix44 transformASTEROID, Matrix44 rotationPlanet,
	Matrix44 rotationSun, Matrix44 rotationAst, Vector3 axisRotation) {

	//Light and Material
	light->whiteLight("spotlight");
	material->pearl("material1");

	//light->setPosition(Vector3(100.0f, 0.0f, 10.0f));
	//Renders
	light->setPosition(camera->eye); // we set the light with the camera
	renderShader6(SUN, rotationSun, cos(M_PI / 16.0f), cos(M_PI / 10.0f));
	renderShader6(MOON, transformMOON*rotationPlanet, cos(M_PI / 16.0f), cos(M_PI / 10.0f));
	renderShader6(ASTEROID, transformASTEROID*rotationAst, cos(M_PI / 16.0f), cos(M_PI / 10.0f));
	renderShader6(PLANET, transformPlanet*rotationPlanet, cos(M_PI / 16.0f), cos(M_PI / 10.0f));
}

void Application::cartoon(Matrix44 transformPlanet, Matrix44 transformMOON, Matrix44 transformASTEROID, Matrix44 rotationPlanet, 
	Matrix44 rotationSun, Matrix44 rotationAst,Vector3 axisRotation) {
	
	Vector3 color;
	//Light and Material
	light->whiteLight("cartoon");
	material->pearl("material1");
	light->setPosition(Vector3(300.0f, 0.0f, 10.0f));

	//light->setPosition(camera->eye);
	//Renders
	//light->setPosition(lighttransform);
	color = Vector3(1.0, 0.5, 0.31);
	renderShader7(SUN, rotationSun,color);
	color = Vector3(0.8, 0.8, 0.8);
	renderShader7(MOON, transformMOON*rotationPlanet,color);
	renderShader7(ASTEROID, transformASTEROID*rotationAst,color);
	color = Vector3(0.0, 0.74, 1.0);
	renderShader7(PLANET, transformPlanet*rotationPlanet,color);
	//aseFiles[LIGHT]->centroid = light->getPosition();

}

void Application::multiplelights(Matrix44 transformPlanet, Matrix44 transformMOON, Matrix44 transformASTEROID, Matrix44 rotationPlanet,
	Matrix44 rotationSun, Matrix44 rotationAst, Vector3 axisRotation) {

	//Light and Material
	//material->pearl("material1");

	//light->setPosition(Vector3(100.0f, 0.0f, 10.0f));
	//Renders
	//light->setPosition(camera->eye); // we set the light with the camera
	renderShader9(SUN, rotationSun);
	renderShader9(MOON, transformMOON*rotationPlanet);
	renderShader9(ASTEROID, transformASTEROID*rotationAst);
	renderShader9(PLANET, transformPlanet*rotationPlanet);
}

void Application::normalMapping() {

	Matrix44 identity, transform;
	transform.setTranslation(200, 0, 0);
	light->whiteLight2("light1");
	light->setPosition(camera->eye);
	material->pearl("material1");

	renderShader8(TEST, identity);

	light->whiteLight2("spotlight");
	material->pearl("material1");
	renderShader6(TEST, transform, cos(M_PI / 16.0f), cos(M_PI / 4.0f));

}

void Application::SwitchingShader(const int aseIndex, const Matrix44& transform) {

	Matrix44 transformPlanet, transformMOON, transformASTEROID, rotationPlanet, rotationSun, rotationAst;
	Vector3 axisRotation(0, 0.1, 0);

	transformPlanet.setTranslation(350 * cos(time*M_PI / 80.0f), 0.0, 350 * sin(time*M_PI / 80.0f));
	transformMOON.setTranslation(350 * cos(time*M_PI / 80.0f) + 150 * cos(time*M_PI / 5.0f), 0.0, 350 * sin(time*M_PI / 80.0f) + 150 * sin(time*M_PI / 5.0f));
	transformASTEROID.setTranslation(-200 * sin(time*M_PI / 5.0f), -200 * cos(time*M_PI / 5.0f), -200 * sin(time*M_PI / 5.0f));

	rotationSun.setRotation(-time * M_PI / 20.0f, axisRotation);
	rotationPlanet.setRotation(time * M_PI / 10.0f, axisRotation);
	rotationAst.setRotation(time * M_PI / 6.0f, axisRotation);

	Matrix44 lighttransform, A, identity;
	A = light->getMatrix();

	switch (shaderMode)
	{
		case 1:
			renderShader1(aseIndex, transform);
			break;
		case 2:
			renderShader2(aseIndex, transform);
			break;
		case 3:
			renderShader3(aseIndex, transform);
			break;
		case 4:
			directionalLight(transformPlanet, transformMOON, transformASTEROID, rotationPlanet, rotationSun, rotationAst, axisRotation);
			renderBackgroud();
			if (b_renderLight) renderShader1(LIGHT, A);
			break;
		case 5:
			pointLight(transformPlanet, transformMOON, transformASTEROID, rotationPlanet, rotationSun, rotationAst, axisRotation);
			renderBackgroud();
			if (b_renderLight) renderShader1(LIGHT, A);
			break;
		case 6:
			spotLight(transformPlanet, transformMOON, transformASTEROID, rotationPlanet, rotationSun, rotationAst, axisRotation);
			renderBackgroud();
			break;
		case 7:
			cartoon(transformPlanet, transformMOON, transformASTEROID, rotationPlanet, rotationSun, rotationAst, axisRotation);
			renderBackgroud();
			if(b_renderLight) renderShader1(LIGHT, A);
			break;
		case 8:
			normalMapping();
			break;
		case 9:
			multiplelights(transformPlanet, transformMOON, transformASTEROID, rotationPlanet, rotationSun, rotationAst, axisRotation);
			renderBackgroud();
			if (b_renderLight) {
				for (int i = 0; i < v_lights.size(); i++) {
					Matrix44 movement;
					movement = v_lights[i]->getMatrix();
					renderShader1(LIGHT, movement);
				}
			}
			break;

	}
}

void Application::renderBackgroud() {
	Matrix44 identity;
	identity.setTranslation(-1000, -1000, -1000);
	renderShader3(BACK, identity);
}

void Application::renderShader1( const int aseIndex, const Matrix44& transform )
{
	// EXERCISE PRACT 3 - Code example for the students
	shader1.enable();


	int position = shader1.getAttribLocation( "Position" );
	

	shader1.setMatrix44( "mvp", camera->viewprojection_matrix );

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation( aseFiles[ aseIndex ]->centroid.x, aseFiles[ aseIndex ]->centroid.y, aseFiles[ aseIndex ]->centroid.z );
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	shader1.setMatrix44( "transform", invTanslation2Centroid * transform * tanslation2Centroid );

	glEnableVertexAttribArrayARB( position );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id );
	glVertexAttribPointerARB( position, 3, GL_FLOAT, GL_FALSE, sizeof( Vector3 ), 0 );

	aseFiles[aseIndex]->render( static_cast<std::underlying_type<PoligonMode>::type>( poligonMode ) );

	shader1.disable();
	// EXERCISE PRACT 3 - Code example for the students
}

void Application::renderShader2(const int aseIndex, const Matrix44& transform) {

	shader2.enable();

	int position = shader2.getAttribLocation("Position");
	int normal = shader2.getAttribLocation("Normal");

	shader2.setMatrix44("mvp", camera->viewprojection_matrix);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	shader2.setMatrix44("transform", invTanslation2Centroid * transform * tanslation2Centroid);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(normal);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[ aseIndex ]->normals_vbo_id);
	glVertexAttribPointerARB(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader2.disable();

}

void Application::renderShader3(const int aseIndex, const Matrix44& transform) {
	
	shader3.enable();

	int position = shader3.getAttribLocation("Position");
	int texture = shader3.getAttribLocation("UVCoord");

	shader3.setMatrix44("mvp", camera->viewprojection_matrix);

	shader3.setTexture("TextureImg", *aseTextures[aseIndex]);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	shader3.setMatrix44("transform", invTanslation2Centroid * transform * tanslation2Centroid);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(texture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader3.disable();

}

void Application::renderShader4(const int aseIndex, const Matrix44& transform) {
	//Directional Light - Vector L do NOT depend on the vertex position 
	shader4.enable();

	int position = shader4.getAttribLocation("a_position");
	int normal = shader4.getAttribLocation("a_normal");
	int texture = shader4.getAttribLocation("a_uv");

	shader4.setMatrix44("mvp", camera->viewprojection_matrix);

	shader4.setTexture("u_textureimg", *aseTextures[aseIndex]);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	Matrix44 model_matrix = invTanslation2Centroid * transform*tanslation2Centroid;
	Matrix44 normalMatrix = model_matrix;
	normalMatrix.inverse();
	normalMatrix.transpose();

	//https://learnopengl.com/Lighting/Basic-Lighting
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/ model matrices and projection matrices
	shader4.setMatrix44("a_normalMatrix", normalMatrix);
	shader4.setMatrix44("transform", model_matrix);
	shader4.setUniform3("a_cameraposition", camera->eye);

	light->bind(shader4);
	material->bind(shader4);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(normal);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->normals_vbo_id);
	glVertexAttribPointerARB(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(texture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader4.disable();
}

void Application::renderShader5(const int aseIndex, const Matrix44& transform) {
	//Directional Light - Vector L do NOT depend on the vertex position 
	shader5.enable();

	int position = shader5.getAttribLocation("a_position");
	int normal = shader5.getAttribLocation("a_normal");
	int texture = shader5.getAttribLocation("a_uv");

	shader5.setMatrix44("mvp", camera->viewprojection_matrix);

	shader5.setTexture("u_textureimg", *aseTextures[aseIndex]);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	Matrix44 model_matrix = invTanslation2Centroid * transform*tanslation2Centroid;
	Matrix44 normalMatrix = model_matrix;
	normalMatrix.inverse();
	normalMatrix.transpose();

	//https://learnopengl.com/Lighting/Basic-Lighting
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/ model matrices and projection matrices
	shader5.setMatrix44("a_normalMatrix", normalMatrix);
	shader5.setMatrix44("transform", model_matrix);
	shader5.setUniform3("a_cameraposition", camera->eye);

	light->bind(shader5);
	material->bind(shader5);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(normal);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->normals_vbo_id);
	glVertexAttribPointerARB(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(texture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader5.disable();
}

void Application::renderShader6(const int aseIndex, const Matrix44& transform, float f_inagle,float f_outangle) {
	//Spotlight - Final light depends on two cut-off angles
	shader6.enable();

	int position = shader6.getAttribLocation("a_position");
	int normal = shader6.getAttribLocation("a_normal");
	int texture = shader6.getAttribLocation("a_uv");

	shader6.setMatrix44("mvp", camera->viewprojection_matrix);

	shader6.setTexture("u_textureimg", *aseTextures[aseIndex]);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	Matrix44 model_matrix = invTanslation2Centroid * transform*tanslation2Centroid;
	Matrix44 normalMatrix = model_matrix;
	normalMatrix.inverse();
	normalMatrix.transpose();

	//https://learnopengl.com/Lighting/Basic-Lighting
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/ model matrices and projection matrices
	shader6.setMatrix44("a_normalMatrix", normalMatrix);
	shader6.setMatrix44("transform", model_matrix);
	shader6.setUniform3("a_cameraposition", camera->eye);

	light->bind(shader6);
	material->bind(shader6);
	
	//Spot Directions and cut-off angles, here we are going to pass the cosines values of the angles
	Vector3 spotdir = light->getPosition()-camera->center  ;
	shader6.setFloat("f_angleInner", f_inagle); 
	shader6.setFloat("f_angleOutter", f_outangle); 
	shader6.setUniform3("v3_spotdir", spotdir);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(normal);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->normals_vbo_id);
	glVertexAttribPointerARB(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(texture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader6.disable();
}

void Application::renderShader7(const int aseIndex, const Matrix44& transform, Vector3 basecolor) {
	//Toon or Cel Shader
	shader7.enable();

	int position = shader7.getAttribLocation("a_position");
	int normal = shader7.getAttribLocation("a_normal");
	//int texture = shader7.getAttribLocation("a_uv");

	shader7.setMatrix44("mvp", camera->viewprojection_matrix);

	//shader7.setTexture("u_textureimg", *aseTextures[aseIndex]);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	Matrix44 model_matrix = invTanslation2Centroid * transform*tanslation2Centroid;
	Matrix44 normalMatrix = model_matrix;
	normalMatrix.inverse();
	normalMatrix.transpose();

	//https://learnopengl.com/Lighting/Basic-Lighting
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/ model matrices and projection matrices
	shader7.setMatrix44("a_normalMatrix", normalMatrix);
	shader7.setMatrix44("transform", model_matrix);
	shader7.setUniform3("a_cameraposition", camera->eye);
	shader7.setUniform3("v3_color", basecolor);

	light->setPosition(Vector3(300.0f*cos(time*M_PI/5.0f), 300.0f*sin(time*M_PI/5.0f), 0.0));
	light->bind(shader7);
	//material->bind(shader7);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(normal);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->normals_vbo_id);
	glVertexAttribPointerARB(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	//glEnableVertexAttribArrayARB(texture);
	//glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	//glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader7.disable();
}

void Application::renderShader8(const int aseIndex, const Matrix44& transform) {
	shader8.enable();

	int position = shader8.getAttribLocation("a_position");
	int normal = shader8.getAttribLocation("a_normal");
	int texture = shader8.getAttribLocation("a_uv");
	int tangent = shader8.getAttribLocation("a_tangent");
	int bitangent = shader8.getAttribLocation("a_bitangent");

	shader8.setMatrix44("mvp", camera->viewprojection_matrix);

	shader8.setTexture("u_textureimg", *aseTextures[aseIndex]);
	shader8.setTexture("u_texturenormal", *aseTextures[aseIndex + 1]);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	Matrix44 model_matrix = invTanslation2Centroid * transform*tanslation2Centroid;
	Matrix44 normalMatrix = model_matrix;
	normalMatrix.inverse();
	normalMatrix.transpose();

	//https://learnopengl.com/Lighting/Basic-Lighting
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/ model matrices and projection matrices
	shader8.setMatrix44("a_normalMatrix", normalMatrix);
	shader8.setMatrix44("transform", model_matrix);
	shader8.setUniform3("a_cameraposition", camera->eye);

	light->bind(shader8);
	material->bind(shader8);

	Vector3 spotdir = light->getPosition() - camera->center;
	shader8.setFloat("f_angleInner", cos(M_PI / 16.0f)); // 16,
	shader8.setFloat("f_angleOutter", cos(M_PI / 4.0f)); //10
	shader8.setUniform3("v3_spotdir", spotdir);

	//Spot Directions and cut-off angles, here we are going to pass the cosines values of the angles
	//Vector3 spotdir = light->getPosition() - camera->center;

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(normal);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->normals_vbo_id);
	glVertexAttribPointerARB(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(texture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	glEnableVertexAttribArrayARB(tangent);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->tangent_vbo_id);
	glVertexAttribPointerARB(tangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(bitangent);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->bitangent_vbo_id);
	glVertexAttribPointerARB(bitangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader8.disable();
}

void Application::renderShader9(const int aseIndex, const Matrix44& transform) {

	// Light movement
	shader9.enable();

	int position = shader9.getAttribLocation("a_position");
	int normal = shader9.getAttribLocation("a_normal");
	int texture = shader9.getAttribLocation("a_uv");

	shader9.setMatrix44("mvp", camera->viewprojection_matrix);

	shader9.setTexture("u_textureimg", *aseTextures[aseIndex]);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	Matrix44 model_matrix = invTanslation2Centroid * transform*tanslation2Centroid;
	Matrix44 normalMatrix = model_matrix;
	normalMatrix.inverse();
	normalMatrix.transpose();

	//https://learnopengl.com/Lighting/Basic-Lighting
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/ model matrices and projection matrices
	shader9.setMatrix44("a_normalMatrix", normalMatrix);
	shader9.setMatrix44("transform", model_matrix);
	shader9.setUniform3("a_cameraposition", camera->eye);

	//light->bind(shader6);
	for (int i = 0; i < v_lights.size(); i++) {
		if (i == 0)
			v_lights[i]->setPosition(Vector3(300.0f*cos(time*M_PI), 300.0f*sin(time*M_PI), 0.0));
		else if (i == 1)
			v_lights[i]->setPosition(Vector3(-300.0f*cos(time*M_PI), -300.0f*sin(time*M_PI), 0.0));


		v_lights[i]->bind(shader9);
	}
	material->bind(shader9);

	//Spot Directions and cut-off angles, here we are going to pass the cosines values of the angles
	//Vector3 spotdir = light->getPosition() - camera->center;
	//shader9.setFloat("f_angleInner", cos(M_PI / 16.0f)); // 16,
	//shader9.setFloat("f_angleOutter", cos(M_PI / 10.0f)); //10
	//shader9.setUniform3("v3_spotdir", spotdir);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(normal);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->normals_vbo_id);
	glVertexAttribPointerARB(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(texture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader9.disable();
}

void Application::update( float seconds_elapsed )
{
	// EXERCISE PRACT 1
	float speed = seconds_elapsed * 100; //the speed is defined by the seconds_elapsed so it goes constant

										 //mouse input to rotate the cam
	if( mouse_state & SDL_BUTTON( SDL_BUTTON_LEFT ) ) //is left button pressed?
	{
		camera->rotate( mouse_delta.x * 0.005f, Vector3( 0, -1, 0 ) );
		camera->rotate( mouse_delta.y * 0.005f, camera->getLocalVector( Vector3( -1, 0, 0 ) ) );
	}

	//if( mouse_state & SDL_BUTTON( SDL_BUTTON_RIGHT ) ) //is RIGHT button pressed?
	//{
	//	camera->move( Vector3( 0, -mouse_delta.y * 0.1f, 0 ) );
	//	camera->move( Vector3( mouse_delta.x * 0.1f, 0, 0 ) );
	//}

	//async input to move the camera around
	if( keystate[ SDL_SCANCODE_LSHIFT ] ) speed *= 10; //move faster with left shift
	if( keystate[ SDL_SCANCODE_W ] || keystate[ SDL_SCANCODE_UP ] ) camera->move( Vector3( 0, 0, -1 ) * speed );
	if( keystate[ SDL_SCANCODE_S ] || keystate[ SDL_SCANCODE_DOWN ] ) camera->move( Vector3( 0, 0, 1 ) * speed );
	if( keystate[ SDL_SCANCODE_A ] || keystate[ SDL_SCANCODE_LEFT ] ) camera->move( Vector3( -1, 0, 0 ) * speed );
	if( keystate[ SDL_SCANCODE_D ] || keystate[ SDL_SCANCODE_RIGHT ] ) camera->move( Vector3( 1, 0, 0 ) * speed );
	// EXERCISE PRACT 1
}

//Keyboard event handler (sync input)
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	switch( event.keysym.sym )
	{
		case SDLK_ESCAPE: exit( 0 ); //ESC key, kill the app

		// EXERCISE PRACT 2
		case SDLK_PLUS:
		case SDLK_KP_PLUS:
			++aseIndex %= (int)aseFiles.size();
			break;
		case SDLK_MINUS:
		case SDLK_KP_MINUS:
			aseIndex = ( --aseIndex < 0 ) ? (int)aseFiles.size() - 1 : aseIndex;
			break;
		case SDLK_1:
			shaderMode = 1;
			break;
		case SDLK_2:
			shaderMode = 2;
			break;
		case SDLK_3:
			shaderMode = 3;
			break;
		case SDLK_4:
			shaderMode = 4;
			break;
		case SDLK_5:
			shaderMode = 5;
			break;
		case SDLK_6:
			shaderMode = 6;
			break;
		case SDLK_7:
			shaderMode = 7;
			break;
		case SDLK_0:
			shaderMode = 0;
			break;
		case SDLK_8:
			shaderMode = 8;
			break;
		case SDLK_9:
			shaderMode = 9;
			break;
		case SDLK_x:
			b_renderLight = !b_renderLight;
			break;
		case SDLK_z:
			camera->getPosition();
			break;
			// EXERCISE PRACT 2
	}

	// EXERCISE PRACT 2
	switch( event.keysym.scancode )
	{
		case SDL_SCANCODE_P:
			poligonMode = PoligonMode::POINT;
			break;

		case SDL_SCANCODE_L:
			poligonMode = PoligonMode::LINE;
			break;

		case SDL_SCANCODE_F:
			poligonMode = PoligonMode::FILL;
			break;
	}
	// EXERCISE PRACT 2
}


void Application::onMouseButton( SDL_MouseButtonEvent event )
{

}

void Application::setWindowSize( int width, int height )
{
	std::cout << "window resized: " << width << "," << height << std::endl;

	// EXERCISE PRACT 1
	glViewport( 0, 0, width, height );
	window_width = width;
	window_height = height;
	camera->aspect = width / (float)height;
	// EXERCISE PRACT 1
}

