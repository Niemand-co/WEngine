#include "pch.h"
#include "Scene/Components/Public/DirectionalLightComponent.h"
#include "Scene/Components/Public/CameraComponent.h"
#include "Render/Public/CascadedShadowMap.h"

DirectionalLightComponent::DirectionalLightComponent(GameObject* inOwner)
	: LightComponent(inOwner)
{
}

void DirectionalLightComponent::UpdateShadowFrustum(CameraInfo* info)
{
	WEngine::CascadedShadowMap::UpdateSplices(m_cascadedShadowMapRanges.GetData(), m_cascadedShadowMapRanges.Size(), info->NearClip, info->FarClip);
	WEngine::CascadedShadowMap::UpdatePSSMMatrices(m_cascadedShadowMapMatrices, glm::inverse(info->MatrixVP), m_pGameObject->GetComponent<TransformComponent>()->GetForward(), m_cascadedShadowMapRanges.GetData(), m_mainLightCascadedShadowMapNum);
}
