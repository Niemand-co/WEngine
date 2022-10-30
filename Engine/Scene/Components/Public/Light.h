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

	Light(GameObject* pGameObject);

	virtual ~Light();

private:

	LightType m_type;

	glm::vec3 m_color;

	float m_intensity;

};