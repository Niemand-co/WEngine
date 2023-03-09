#include "pch.h"
#include "Render/Renderer/Public/ScriptableRenderer.h"
#include "Scene/Components/Public/CameraComponent.h"
#include "Scene/Components/Public/PrimitiveComponent.h"

ScriptableRenderer::ScriptableRenderer(CameraComponent* pCamera, WViewport* Viewport)
	: m_pCamera(pCamera),
	  Viewport(Viewport)
{
}

ScriptableRenderer::~ScriptableRenderer()
{
}

void SceneRenderer::GatherDynamicMeshElements()
{
	const WEngine::WArray<PrimitiveInfo*>& Primitives = Scene->GetPrimitives();
	const WEngine::WArray<uint8>& PrimitiveMasks = Scene->GetPrimitiveMasks();
	uint32 NumPrimitive = Primitives.Size();

	for (uint32 PrimitiveIndex = 0; PrimitiveIndex < NumPrimitive; ++PrimitiveIndex)
	{
		if (PrimitiveMasks[PrimitiveIndex] & (uint8)PrimitiveInfo::PrimitiveMask::Dynamic > 0)
		{
			Primitives[PrimitiveIndex]->Proxy->GetDynamicMeshElements(Views, Collector);
		}
	}
}
