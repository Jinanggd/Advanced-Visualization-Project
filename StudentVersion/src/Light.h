#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include <sstream>
#include <vector>
#include <Shader.h>
#include "Maths.h"

class Light {

	std::string m_sLightName; // Light Id used in the shader
	Vector3 m_v3Position; // Light position in eye coords
	Vector3 m_v3La; // Ambient light intensity
	Vector3 m_v3Ld; // Diffuse light intensity
	Vector3 m_v3Ls; // Specular light intensity

	
public:
	Light();
	Light(std::string LightID, Vector3 Position, Vector3 AmbientLightInt, Vector3 DiffuseLightInt, Vector3 SpecularLightInt);
	~Light();

	void redLight(std::string lightID);
	void whiteLight(std::string lightID);
	void blueLight(std::string lightID);
	void greenLight(std::string lightID);
	void whiteLight2(std::string lightID);


	Vector3 getPosition();
	void setPosition(Vector3 position);
	void setPosition(Matrix44 transform);
	void bind(Shader& shader) const;

	Matrix44 getMatrix();
};


#endif
