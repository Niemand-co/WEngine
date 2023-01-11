#include "pch.h"
#include "Render/Public/Scene.h"
#include "Scene/Components/Public/InstancedStaticMeshComponent.h"

RScene::RScene()
{
}

RScene::~RScene()
{
}

void RScene::AddPrimitive(PrimitiveComponent* primitive)
{
	m_addedPrimitives.Add(primitive->GetPrimitiveInfo());
}

void RScene::RemovePrimitive(PrimitiveComponent* primitive)
{
	m_removedPrimitives.Remove(primitive->GetPrimitiveInfo());
}

void RScene::UpdatePrimitiveInfosForScene()
{
	WEngine::WArray<PrimitiveInfo*> AddedPrimitives = m_addedPrimitives.Array();

	WEngine::WArray<PrimitiveInfo*> RemovedPrimitives = m_removedPrimitives.Array();


}
