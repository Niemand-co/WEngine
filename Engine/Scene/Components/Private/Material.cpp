#include "pch.h"
#include "Scene/Components/Public/Material.h"
#include "RHI/Public/RHIShader.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Scene/Public/GameObject.h"

Material::Material(GameObject* pGameObject)
	: Component(pGameObject), albedo(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), roughness(0.5f), metallic(0.0f), m_shader(nullptr)
{
	m_type = Component::ComponentType::Material;
	albedoPropertyID = "##" + pGameObject->GetName() + "_Albedo";
	roughnessPropertyID = "##" + pGameObject->GetName() + "_Roughness";
	metallicPropertyID = "##" + pGameObject->GetName() + "_Metallic";
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

void Material::SetColor(glm::vec4 color)
{
	albedo = color;
}

void Material::SetRoughness(float roughness)
{
	this->roughness = roughness;
}

void Material::ShowInInspector()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Roughness"); ImGui::SameLine();
		ImGui::SliderFloat(roughnessPropertyID.c_str(), &roughness, 0.01f, 1.0f);

		ImGui::Text("Metallic"); ImGui::SameLine();
		ImGui::SliderFloat(metallicPropertyID.c_str(), &metallic, 0.0f, 1.0f);

		ImGui::Text("Albedo"); ImGui::SameLine();
		ImGui::ColorEdit3(albedoPropertyID.c_str(), &albedo[0]);
	}
}
