#include "pch.h"
#include "Scene/Components/Public/InstancedStaticMeshComponent.h"

InstancedStaticMeshComponent::InstancedStaticMeshComponent(GameObject* pGameObject)
	: StaticMeshComponent(pGameObject), m_instancedCount(0)
{
}

InstancedStaticMeshComponent::~InstancedStaticMeshComponent()
{
}

PrimitiveInfo* InstancedStaticMeshComponent::GetPrimitiveInfo()
{
	if (!m_pMesh)
	{
		return nullptr;
	}

	if (m_bMarkedDirty)
	{
		if (m_pInfo == nullptr)
		{
			WEngine::Allocator::Get()->Allocate(sizeof(InstancedStaticMeshComponent));
		}
		::new (m_pInfo) InstancedStaticMeshInfo(this);
		m_bMarkedDirty = false;
	}

	return m_pInfo;
}

void InstancedStaticMeshComponent::AddInstance()
{
	m_instancedCount++;
	TransformComponent *newTransform = GetCopy(m_pGameObject->GetComponent<TransformComponent>());
	m_transforms.Push(newTransform);
}
