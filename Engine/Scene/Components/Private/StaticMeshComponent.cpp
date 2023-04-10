#include "pch.h"
#include "Scene/Components/Public/StaticMeshComponent.h"
#include "Render/Mesh/Public/MeshBatch.h"
#include "Render/Public/SceneView.h"
#include "Scene/Public/SceneManagement.h"
#include "Scene/Public/World.h"

StaticMeshComponent::StaticMeshComponent(GameObject* pGameObject, WStaticMesh* pMesh)
	: PrimitiveComponent(pGameObject), pMesh(pMesh)
{
	if (pMesh != nullptr)
	{
		Proxy = (StaticMeshProxy*)NormalAllocator::Get()->Allocate(sizeof(StaticMeshProxy));
		::new (Proxy) StaticMeshProxy(this);
		GWorld::GetWorld()->Scene->AddPrimitive(this);
	}
}

StaticMeshComponent::~StaticMeshComponent()
{
}

PrimitiveProxy* StaticMeshComponent::GetPrimitiveProxy()
{
	if (!pMesh)
	{
		return nullptr;
	}

	if (bMarkedDirty)
	{
		::new (Proxy) StaticMeshProxy(this);
		bMarkedDirty = false;
	}

	return Proxy;
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
			Element.FirstIndex = 0;
			Element.NumPrimitives = RenderData->LodResources[LodIndex].NumIndices / 3;
			Element.NumInstances = 1;
		}
	}
}

void StaticMeshProxy::GenerateBoundingBox()
{
	Mesh->GenerateBoundingBox();
}