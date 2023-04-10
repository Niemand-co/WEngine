#include "pch.h"
#include "Scene/Components/Public/PrimitiveComponent.h"
#include "Scene/Public/SceneManagement.h"
#include "Scene/Public/World.h"

PrimitiveComponent::PrimitiveComponent(GameObject* pGameObject)
    : Component(pGameObject),
      bVisible(true),
      bTranslucent(false),
      bCastShadow(true),
      bStatic(false),
      Proxy(nullptr)
{
}

PrimitiveComponent::~PrimitiveComponent()
{
}
