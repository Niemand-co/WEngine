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

	const std::vector<RHITextureView*>& GetDepthTexture() const { return m_pDepthTextureViews; }

	std::vector<glm::mat4> GetShadowFrustum(CameraData* cameraData);

private:

	LightType m_type;

	bool m_isMainLight;

	glm::vec3 m_color;

	float m_intensity;

	std::vector<RHITexture*> m_pDepthTextures;

	std::vector<RHITextureView*> m_pDepthTextureViews;

	unsigned int m_mainLightCascadedShadowMapNum = 4;

	std::vector<float> m_mainLightCascadedShadowMapRange;

	std::vector<glm::mat4> m_lightSpaceMatrix;

};