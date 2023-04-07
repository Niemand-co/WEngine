#include "pch.h"
#include "Scene/Components/Public/PrimitiveComponent.h"
#include "Scene/Public/SceneManagement.h"
#include "Scene/Public/World.h"

PrimitiveComponent::PrimitiveComponent(GameObject* pGameObject)
    : Component(pGameObject),
      m_bVisible(true),
      m_bTranslucent(false),
      m_bCastShadow(true),
      m_bStatic(false),
      m_pProxy(nullptr)
{
    GWorld::GetWorld()->Scene->AddPrimitive(this);
}

PrimitiveComponent::~PrimitiveComponent()
{
}
