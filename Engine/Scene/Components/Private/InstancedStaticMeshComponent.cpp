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
	if (!pMesh)
	{
		return nullptr;
	}

	if (bMarkedDirty)
	{
		if (Proxy == nullptr)
		{
			Proxy = (InstancedStaticMeshProxy*)GetCPUAllocator()->Allocate(sizeof(InstancedStaticMeshProxy));
		}
		::new (Proxy) InstancedStaticMeshProxy(this);
		bMarkedDirty = false;
	}

	return Proxy;
}

void InstancedStaticMeshComponent::AddInstance()
{
	m_instancedCount++;
	TransformComponent *newTransform = GetCopy(pGameObject->GetComponent<TransformComponent>());
	m_transforms.Push(newTransform);
}
