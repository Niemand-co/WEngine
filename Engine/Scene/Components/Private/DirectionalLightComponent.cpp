#include "pch.h"
#include "Scene/Components/Public/DirectionalLightComponent.h"
#include "Scene/Components/Public/CameraComponent.h"
#include "Render/Public/CascadedShadowMap.h"

DirectionalLightComponent::DirectionalLightComponent(GameObject* inOwner)
	: LightComponent(inOwner)
{
}
