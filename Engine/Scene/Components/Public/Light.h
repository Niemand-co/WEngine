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

	void SetMainLight(bool isMainLight);

	virtual void ShowInInspector() override;

private:

	LightType m_type;

	bool m_isMainLight;

	glm::vec3 m_color;

	float m_intensity;

};