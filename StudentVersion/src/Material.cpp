#include <Material.h>

Material::Material()
	: m_sMaterialName("material1")
	, m_v3Ka(0.5, 0.3, 0.5)
	, m_v3Kd(0.5,0.5,0.8)
	, m_v3Ks(0.5,0.727811,0.5)
	, m_fShininess(10*128)
{
}

Material::Material(std::string MaterialID, Vector3 AmbientReflec, Vector3 DiffuseReflec, Vector3 SpecularReflec, float Shininess) {
	m_sMaterialName = MaterialID;
	m_v3Ka = AmbientReflec;
	m_v3Kd = DiffuseReflec;
	m_v3Ks = SpecularReflec;
	m_fShininess = Shininess;
}

//Definir uns materials per defecte

void Material::esmerald(std::string MaterialID) {
	m_sMaterialName = MaterialID;
	m_v3Ka = Vector3(0.0215, 0.145, 0.0215);
	m_v3Kd = Vector3(0.07568, 0.61424, 0.07568);
	m_v3Ks = Vector3(0.633, 0.727811, 0.633);
	m_fShininess = 0.6 * 128.0;
}

void Material::jade(std::string MaterialID) {
	m_sMaterialName = MaterialID;
	m_v3Ka = Vector3(0.135, 0.2225, 0.1575);
	m_v3Kd = Vector3(0.54, 0.89, 0.63);
	m_v3Ks = Vector3(0.316228, 0.316228, 0.316228);
	m_fShininess = 0.1*128.0;
}

void Material::pearl(std::string MaterialID) {
	m_sMaterialName = MaterialID;
	m_v3Ka = Vector3(0.25, 0.20725, 0.20725);
	m_v3Kd = Vector3(1.0, 0.829, 0.829);
	m_v3Ks = Vector3(0.296648, 0.296648, 0.296648);
	m_fShininess = 0.088*128.0;
}

void Material::bind(Shader& shader)const {

	shader.setUniform3(m_sMaterialName + ".Ka", m_v3Ka);
	shader.setUniform3(m_sMaterialName + ".Kd", m_v3Kd);
	shader.setUniform3(m_sMaterialName + ".Ks", m_v3Ks);
	shader.setFloat(m_sMaterialName + ".Shininess", m_fShininess);

}