#include "pch.h"
#include "Scene/Components/Public/PrimitiveComponent.h"

PrimitiveComponent::PrimitiveComponent(GameObject* pGameObject)
    : Component(pGameObject)
{
    m_pInfo = nullptr;
}

PrimitiveComponent::~PrimitiveComponent()
{
}
