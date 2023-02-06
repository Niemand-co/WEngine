#include "pch.h"
#include "Scene/Components/Public/InstancedStaticMeshComponent.h"

InstancedStaticMeshComponent::InstancedStaticMeshComponent(GameObject* pGameObject)
	: StaticMeshComponent(pGameObject), m_instancedCount(0)
{
}

InstancedStaticMeshComponent::~InstancedStaticMeshComponent()
{
}

PrimitiveProxy* InstancedStaticMeshComponent::GetPrimitiveProxy()
{
	if (!m_pMesh)
	{
		return nullptr;
	}

	if (m_bMarkedDirty)
	{
		if (m_pProxy == nullptr)
		{
			m_pProxy = (InstancedStaticMeshProxy*)WEngine::Allocator::Get()->Allocate(sizeof(InstancedStaticMeshProxy));
		}
		::new (m_pProxy) InstancedStaticMeshProxy(this);
		m_bMarkedDirty = false;
	}

	return m_pProxy;
}

void InstancedStaticMeshComponent::AddInstance()
{
	m_instancedCount++;
	TransformComponent *newTransform = GetCopy(m_pGameObject->GetComponent<TransformComponent>());
	m_transforms.Push(newTransform);
}
