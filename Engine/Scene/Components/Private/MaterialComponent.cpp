#include "pch.h"
#include "Scene/Components/Public/MaterialComponent.h"
#include "RHI/Public/RHIShader.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Scene/Public/GameObject.h"

MaterialComponent::MaterialComponent(GameObject* pGameObject)
	: Component(pGameObject), albedo(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), roughness(0.5f), metallic(0.0f), m_shader(nullptr)
{
}

void MaterialComponent::SetShader(RHIShader* shader)
{
	m_shader = shader;
}

RHIShader* MaterialComponent::GetShader()
{
	return m_shader;
}

SurfaceData MaterialComponent::GetSurfaceData()
{
	SurfaceData surfaceData = {};
	surfaceData.albedo = albedo;
	surfaceData.roughness = roughness;
	surfaceData.mettalic = metallic;
	return surfaceData;
}

void MaterialComponent::SetColor(glm::vec4 color)
{
	albedo = color;
}

void MaterialComponent::SetRoughness(float roughness)
{
	this->roughness = roughness;
}

void MaterialComponent::ShowInInspector()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Columns(2); ImGui::SetColumnWidth(0, 80.0f);

		ImGui::Text("Roughness"); ImGui::SameLine(); ImGui::NextColumn();
		ImGui::PushItemWidth(ImGui::CalcItemWidth());
		ImGui::SliderFloat("##Roughness", &roughness, 0.01f, 1.0f); ImGui::NextColumn();
		ImGui::PopItemWidth();

		ImGui::Text("Metallic"); ImGui::SameLine(); ImGui::NextColumn();
		ImGui::PushItemWidth(ImGui::CalcItemWidth());
		ImGui::SliderFloat("##Metallic", &metallic, 0.0f, 1.0f); ImGui::NextColumn();
		ImGui::PopItemWidth();

		ImGui::Text("Albedo"); ImGui::SameLine(); ImGui::NextColumn();
		ImGui::PushItemWidth(ImGui::CalcItemWidth());
		ImGui::ColorEdit3("##Albedo", &albedo[0]); ImGui::NextColumn();
		ImGui::PopItemWidth();

		ImGui::Columns(1);
	}
}
