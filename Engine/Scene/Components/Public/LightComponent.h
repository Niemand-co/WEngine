#pragma once
#include "Scene/Components/Public/Component.h"
#include "Scene/Components/Public/TransformComponent.h"

class RHITexture;
class RHITextureView;
struct CameraInfo;

class LightComponent : public Component
{

	friend class GameObject;

public:
	
	enum { type = 2 };

	LightComponent(GameObject* pGameObject);

	virtual ~LightComponent() = default;

	void SetMainLight(bool isMainLight);

	virtual void ShowInInspector() override;

	float GetIntensity() const { return m_intensity; }

	glm::vec3 GetColor() const { return m_color; }

	const WEngine::WArray<RHITextureView*>& GetDepthTexture() const { return m_pDepthTextureViews; }

	class LightInfo* GetLightInfo();

private:

	bool m_isMainLight;

	glm::vec3 m_color;

	float m_intensity;

	WEngine::WArray<RHITexture*> m_pDepthTextures;

	WEngine::WArray<RHITextureView*> m_pDepthTextureViews;

	LightInfo *m_pInfo;

};

struct LightInfo
{
	LightInfo(LightComponent *light)
		: LightColor(light->GetColor()),
		  LightInstensity(light->GetIntensity()),
		  Owner(light->GetOwner())
	{

	}

	enum { type = 0 };

	glm::vec3 LightColor;

	float LightInstensity;

	GameObject *Owner;
};