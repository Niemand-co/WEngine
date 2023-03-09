#include "pch.h"
#include "Scene/Components/Public/PrimitiveComponent.h"
#include "Scene/Public/SceneManagement.h"

PrimitiveComponent::PrimitiveComponent(GameObject* pGameObject)
    : Component(pGameObject),
      m_bVisible(true),
      m_bTranslucent(false),
      m_bCastShadow(true),
      m_bStatic(false),
      m_pProxy(nullptr)
{
}

PrimitiveComponent::~PrimitiveComponent()
{
}
