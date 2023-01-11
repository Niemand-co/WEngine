#include "pch.h"
#include "Scene/Components/Public/StaticMeshComponent.h"

StaticMeshComponent::StaticMeshComponent(GameObject* pGameObject)
	: PrimitiveComponent(pGameObject), m_pMesh(nullptr)
{
}

StaticMeshComponent::~StaticMeshComponent()
{
}

PrimitiveInfo* StaticMeshComponent::GetPrimitiveInfo()
{
	if (!m_pMesh)
	{
		return nullptr;
	}

	if (m_bMarkedDirty)
	{
		if (m_pInfo == nullptr)
		{
			m_pInfo = (StaticMeshInfo*)WEngine::Allocator::Get()->Allocate(sizeof(StaticMeshInfo));
		}
		::new (m_pInfo) StaticMeshInfo(this);
		m_bMarkedDirty = false;
	}

	return m_pInfo;
}
