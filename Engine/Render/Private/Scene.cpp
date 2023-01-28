#include "pch.h"
#include "Render/Public/Scene.h"
#include "Scene/Components/Public/InstancedStaticMeshComponent.h"

RScene* RScene::g_activeScene = nullptr;

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
	m_addedPrimitives.Clear();

	WEngine::WArray<PrimitiveInfo*> RemovedPrimitives = m_removedPrimitives.Array();
	m_removedPrimitives.Clear();

	for (PrimitiveInfo* removedInfo : RemovedPrimitives)
	{
		uint32 removedCount = 0;
		uint32 size = m_primitives.Size();
		for (uint32 index = 0; index < size; ++index)
		{
			if (m_primitives[index] == removedInfo)
			{
				WEngine::Swap(&m_primitives[index], &m_primitives[size - removedCount - 1]);
				removedCount++;
			}
		}
	}
	m_primitives.Resize(m_primitives.Size() - RemovedPrimitives.Size());

	for (PrimitiveInfo* removedInfo : AddedPrimitives)
	{
		m_primitives.Push(removedInfo);
	}

	WEngine::WArray<PrimitiveInfo*>& primitives = m_primitives;
}
