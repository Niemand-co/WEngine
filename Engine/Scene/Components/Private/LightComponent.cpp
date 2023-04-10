#include "pch.h"
#include "Scene/Components/Public/LightComponent.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Public/World.h"
#include "Render/Public/Scene.h"

LightComponent::LightComponent(GameObject *pGameObject)
	: Component(pGameObject), m_intensity(1.0f), m_color(glm::vec3(1.0f, 1.0f, 1.0f))
{
	GWorld::GetWorld()->Scene->AddLight(this);
}

void LightComponent::SetMainLight(bool isMainLight)
{
	m_isMainLight = isMainLight;
}

void LightComponent::ShowInInspector()
{
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Intensity"); ImGui::SameLine();
		ImGui::SliderFloat("##Intensity", &m_intensity, 0.0f, 10.0f);

		ImGui::Text("Color"); ImGui::SameLine();
		ImGui::ColorEdit3("##LightColor", &m_color[0]);
	}
}

LightInfo* LightComponent::GetLightInfo()
{
	if (bMarkedDirty)
	{
		if (m_pInfo == nullptr)
		{
			m_pInfo = (LightInfo*)NormalAllocator::Get()->Allocate(sizeof(LightInfo));
		}
		::new (m_pInfo) LightInfo(this);
		bMarkedDirty = false;
	}

	return m_pInfo;
}
