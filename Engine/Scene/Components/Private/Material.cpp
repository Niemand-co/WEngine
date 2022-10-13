#include "pch.h"
#include "Scene/Components/Public/Material.h"
#include "RHI/Public/RHIShader.h"

Material::Material()
	: albedo(glm::vec3(1.0f, 1.0f, 1.0f)), roughness(1.0f), metallic(0.0f), m_shader(nullptr)
{
	m_type = Component::ComponentType::Material;
}

void Material::SetShader(RHIShader* shader)
{
	m_shader = shader;
}

RHIShader* Material::GetShader()
{
	return m_shader;
}

SurfaceData Material::GetSurfaceData()
{
	return SurfaceData(albedo, roughness, metallic);
}
