#include "pch.h"
#include "Scene/Components/Public/Light.h"
#include "Scene/Public/GameObject.h"

Light::Light(GameObject *gameObject)
	: m_type(LightType::Directional), m_intensity(1.0f), m_color(glm::vec3(1.0f, 1.0f, 1.0f)), m_pGameObject(gameObject)
{
}

Light::~Light()
{
}