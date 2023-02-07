#pragma once
#include "Scene/Components/Public/LightComponent.h"

class DirectionalLightComponent : public LightComponent
{
public:

	enum { type = 8 };

	DirectionalLightComponent(GameObject *inOwner);

	virtual ~DirectionalLightComponent() = default;

	const WEngine::WArray<float>& GetCSMRanges() const { return m_cascadedShadowMapRanges; }

	const WEngine::WArray<glm::mat4>& GetCSMMatrices() const { return m_cascadedShadowMapMatrices; }

private:

	WEngine::WArray<float> m_cascadedShadowMapRanges;

	WEngine::WArray<glm::mat4> m_cascadedShadowMapMatrices;

	unsigned int m_mainLightCascadedShadowMapNum = 4;

};

struct DirectionalLightInfo : public LightInfo
{
	DirectionalLightInfo(DirectionalLightComponent* light)
		: LightInfo(light),
		  LightSpaceMatrix(light->GetOwner()->GetComponent<TransformComponent>()->GetWorldToLocalMatrix()),
		  LightDirection(light->GetOwner()->GetComponent<TransformComponent>()->GetForward()),
		  CascadedShadowMapMatrices(light->GetCSMMatrices()),
		  CascadedShadowMapRanges(light->GetCSMRanges())
	{

	}

	enum { type = 1 };

	glm::mat4 LightSpaceMatrix;

	glm::vec3 LightDirection;

	WEngine::WArray<glm::mat4> CascadedShadowMapMatrices;

	WEngine::WArray<float> CascadedShadowMapRanges;

};