#include "pch.h"
#include "Scene/Components/Public/Light.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Public/World.h"

Light::Light(GameObject *pGameObject)
	: Component(pGameObject), m_type(LightType::Directional), m_intensity(1.0f), m_color(glm::vec3(1.0f, 1.0f, 1.0f))
{
	World::GetWorld()->AddLight(this);
}

Light::~Light()
{
}

void Light::SetMainLight(bool isMainLight)
{
	m_isMainLight = isMainLight;
}

void Light::ShowInInspector()
{
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Intensity"); ImGui::SameLine();
		ImGui::SliderFloat("##Intensity", &m_intensity, 0.0f, 10.0f);

		ImGui::Text("Color"); ImGui::SameLine();
		ImGui::ColorEdit3("##LightColor", &m_color[0]);
	}
}
