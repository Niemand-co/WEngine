#include "pch.h"
#include "Scene/Components/Public/Material.h"
#include "RHI/Public/RHIShader.h"

Material::Material()
	: color(Vector3(1.0f, 1.0f, 1.0f)), roughness(1.0f), m_shader(nullptr)
{

}

void Material::SetShader(RHIShader* shader)
{
	m_shader = shader;
}

RHIShader* Material::GetShader()
{
	return m_shader;
}