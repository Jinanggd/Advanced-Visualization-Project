#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <sstream>
#include <vector>
#include <Shader.h>
#include "Maths.h"

class Material {
	std::string m_sMaterialName; // Material Id used in the shader
	Vector3 m_v3Ka; // Ambient reflectivity
	Vector3 m_v3Kd; // Diffuse reflectivity
	Vector3 m_v3Ks; // Specular reflectivity
	float m_fShininess; // Specular shininess factor

	
public:
	Material();
	Material(std::string MaterialID, Vector3 AmbientReflec, Vector3 DiffuseReflec, Vector3 SpecularReflec, float Shininess);
	~Material();

	void esmerald(std::string MaterialID);
	void pearl(std::string MaterialID);
	void jade(std::string MaterialID);
	void bind(Shader& shader) const;
};



#endif
