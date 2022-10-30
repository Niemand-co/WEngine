#include "pch.h"
#include "Scene/Components/Public/Material.h"
#include "RHI/Public/RHIShader.h"
#include "Utils/ImGui/Public/Gui.h"

Material::Material(GameObject* pGameObject)
	: Component(pGameObject), albedo(glm::vec3(1.0f, 1.0f, 1.0f)), roughness(0.5f), metallic(0.0f), m_shader(nullptr)
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
	SurfaceData surfaceData = {};
	surfaceData.albedo = albedo;
	surfaceData.roughness = roughness;
	surfaceData.mettalic = metallic;
	return surfaceData;
}

void Material::SetColor(glm::vec3 color)
{
	albedo = color;
}

void Material::SetRoughness(float roughness)
{
	this->roughness = roughness;
}

void Material::ShowInInspector()
{
	Gui::DrawSlider("Roughness", &roughness, 0.01f, 1.0f);

	Gui::DrawSlider("Metallic", &metallic, 0.0f, 1.0f);

	Gui::DrawColorEdit("Albedo", &albedo[0], false);
}
