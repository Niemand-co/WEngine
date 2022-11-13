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

	friend class GameObject;

public:
	
	typedef Light type;

	Light(GameObject* pGameObject);

	virtual ~Light();

	virtual void ShowInInspector() override;

private:

	LightType m_type;

	glm::vec3 m_color;

	float m_intensity;

};