#pragma once
#include "Scene/Components/Public/Component.h"

class GameObject;
class RHITexture;
class RHITextureView;
struct CameraData;

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

	float GetIntensity() const { return m_intensity; }

	glm::vec3 GetColor() const { return m_color; }

	const WEngine::WArray<RHITextureView*>& GetDepthTexture() const { return m_pDepthTextureViews; }

	void UpdateShadowFrustum(CameraData* cameraData);

	const WEngine::WArray<glm::mat4>& GetShadowFrustum();

	const WEngine::WArray<float>& GetSplices();

private:

	LightType m_type;

	bool m_isMainLight;

	glm::vec3 m_color;

	float m_intensity;

	WEngine::WArray<RHITexture*> m_pDepthTextures;

	WEngine::WArray<RHITextureView*> m_pDepthTextureViews;

	unsigned int m_mainLightCascadedShadowMapNum = 4;

	WEngine::WArray<float> m_mainLightCascadedShadowMapRange;

	WEngine::WArray<glm::mat4> m_lightSpaceMatrix;

};