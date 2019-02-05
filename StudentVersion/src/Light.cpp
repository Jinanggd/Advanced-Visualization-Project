#include <Light.h>

Light::Light() 
	: m_sLightName("light1")
	, m_v3Position(300.0f,300.0f,300.0f)
	, m_v3La(1.0,0.0,0.0)
	, m_v3Ld(0.8,0.0,0.0)
	, m_v3Ls(0.8, 0.2, 0.1)
{
}

Light::Light(std::string LightID, Vector3 Position, Vector3 AmbientLightInt, Vector3 DiffuseLightInt, Vector3 SpecularLightInt) {
	m_sLightName = LightID;
	m_v3Position = Position;
	m_v3La = AmbientLightInt;
	m_v3Ld = DiffuseLightInt;
	m_v3Ls = SpecularLightInt;

}

// Definir color light per defecte
void Light::redLight(std::string lightID){
	m_sLightName = lightID;
	m_v3La = Vector3(1.0, 0.0, 0.0);
	m_v3Ld = Vector3(0.8, 0.0, 0.0);
	m_v3Ls = Vector3(0.5, 0.0, 0.0);

}

void Light::blueLight(std::string lightID) {
	m_sLightName = lightID;
	m_v3La = Vector3(0.0, 0.0, 1.0);
	m_v3Ld = Vector3(0.0, 0.0, 0.8);
	m_v3Ls = Vector3(0.0, 0.0, 0.5);
}

void Light::greenLight(std::string lightID) {
	m_sLightName = lightID;
	m_v3La = Vector3(0.0, 1.0, 0.0);
	m_v3Ld = Vector3(0.0, 1.0, 0.0);
	m_v3Ls = Vector3(0.0, 0.5, 0.0);
}
void Light::whiteLight(std::string lightID) {
	m_sLightName = lightID;
	m_v3La = Vector3(1.0, 1.0, 1.0);
	m_v3Ld = Vector3(1.0, 1.0, 1.0);
	m_v3Ls = Vector3(0.5,0.5,0.5);
}

void Light::whiteLight2(std::string lightID) {
	m_sLightName = lightID;
	m_v3La = Vector3(1.0, 1.0, 1.0);
	m_v3Ld = Vector3(1.0, 1.0, 1.0);
	m_v3Ls = Vector3(3, 3, 3);
}



Vector3 Light::getPosition() {
	return m_v3Position;
}
void Light::setPosition(Vector3 position) {
	m_v3Position = position;
}

void Light::setPosition(Matrix44 transform) {
	//Vector3 copy = m_v3Position;

	m_v3Position = transform * m_v3Position;
}

void Light::bind(Shader& shader) const{

	shader.setUniform3(m_sLightName + ".Position", m_v3Position);
	shader.setUniform3(m_sLightName + ".La", m_v3La);
	shader.setUniform3(m_sLightName + ".Ld", m_v3Ld);
	shader.setUniform3(m_sLightName + ".Ls", m_v3Ls);

}

Matrix44 Light::getMatrix() {
	Matrix44 matrixtome;
	matrixtome.setTranslation(m_v3Position.x, m_v3Position.y, m_v3Position.x);
	return matrixtome;
}




