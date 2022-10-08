#pragma once
#include "Scene/Components/Public/Component.h"

class GameObject;

class Light : public Component
{

	enum class LightType
	{
		Directional = 0,
		Point,
		Spot,
	};

public:

	Light(GameObject* gameObject);

	virtual ~Light();

private:

	GameObject *m_pGameObject;

	LightType m_type;

	glm::vec3 m_color;

	float m_intensity;

};