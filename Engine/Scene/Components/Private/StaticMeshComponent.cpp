#include "pch.h"
#include "Scene/Components/Public/StaticMeshComponent.h"

StaticMeshComponent::StaticMeshComponent(GameObject* pGameObject, StaticMesh* pMesh)
	: PrimitiveComponent(pGameObject), m_pMesh(pMesh)
{
	if (pMesh != nullptr)
	{
		RScene::GetActiveScene()->AddPrimitive(this);
	}
}

StaticMeshComponent::~StaticMeshComponent()
{
}

PrimitiveProxy* StaticMeshComponent::GetPrimitiveProxy()
{
	if (!m_pMesh)
	{
		return nullptr;
	}

	if (m_bMarkedDirty)
	{
		if (m_pProxy == nullptr)
		{
			m_pProxy = (StaticMeshProxy*)WEngine::Allocator::Get()->Allocate(sizeof(StaticMeshProxy));
		}
		::new (m_pProxy) StaticMeshProxy(this);
		m_bMarkedDirty = false;
	}

	return m_pProxy;
}

void StaticMeshProxy::DrawStaticMesh(RHICommandListBase* CmdList)
{
	
}

void StaticMeshProxy::GenerateBoundingBox()
{
	Mesh->GenerateBoundingBox();
}
