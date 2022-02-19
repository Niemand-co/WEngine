#include "Material.h"

Material::Material()
	: color(Vec3(1.0f, 1.0f, 1.0f)), roughness(1.0f)
{

}

void Material::SetShader(Shader* shader)
{
	m_shader = shader;
}

Shader* Material::GetShader()
{
	return m_shader;
}