#include "pch.h"
#include "Scene/Components/Public/StaticMeshComponent.h"
#include "Render/Mesh/Public/MeshBatch.h"
#include "Render/Public/SceneView.h"
#include "Scene/Public/SceneManagement.h"

StaticMeshComponent::StaticMeshComponent(GameObject* pGameObject, WStaticMesh* pMesh)
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
			m_pProxy = (StaticMeshProxy*)NormalAllocator::Get()->Allocate(sizeof(StaticMeshProxy));
		}
		::new (m_pProxy) StaticMeshProxy(this);
		m_bMarkedDirty = false;
	}

	return m_pProxy;
}

void StaticMeshProxy::DrawStaticMesh(RHICommandListBase* CmdList)
{
	
}

void StaticMeshProxy::GetDynamicMeshElements(const WEngine::WArray<WSceneViewInfo*>& Views, WMeshCollector& Collector)
{
	for (uint32 ViewIndex = 0; ViewIndex < Views.Size(); ++ViewIndex)
	{
		for (uint32 LodIndex = 0; LodIndex < RenderData->LodResources.Size(); ++LodIndex)
		{
			WMeshBatch& Mesh = Collector.AllocateMesh();
			WMeshBatchElement& Element = Mesh.Elements[0];

			Mesh.VertexFactory = &RenderData->Factories[LodIndex].VertexFactory;

			Element.IndexBuffer = &RenderData->LodResources[LodIndex].IndexBuffer;
		}
	}
}

void StaticMeshProxy::GenerateBoundingBox()
{
	Mesh->GenerateBoundingBox();
}