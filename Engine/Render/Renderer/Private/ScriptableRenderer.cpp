#include "pch.h"
#include "Render/Renderer/Public/ScriptableRenderer.h"
#include "Render/Renderer/Public/DeferredRenderer.h"
#include "Scene/Components/Public/CameraComponent.h"
#include "Scene/Components/Public/PrimitiveComponent.h"
#include "Render/Public/SceneVisibility.h"

ScriptableRenderer::ScriptableRenderer()
{
}

ScriptableRenderer::~ScriptableRenderer()
{
}

SceneRenderer::SceneRenderer(const WSceneViewFamily* InViewFamily)
	: ViewFamily(InViewFamily),
	  Scene(InViewFamily->Scene)
{
	Views.Resize(ViewFamily->Views.Size());
	for (uint32 ViewIndex = 0; ViewIndex < Views.Size(); ++ViewIndex)
	{
		Views[ViewIndex] = WViewInfo(*ViewFamily->Views[ViewIndex]);
		Views[ViewIndex].Family = WEngine::RemoveConst(ViewFamily);
	}
}

void SceneRenderer::PrepareViewForRendering()
{
	for (int ViewIndex = 0; ViewIndex < Views.Size(); ++ViewIndex)
	{
		WViewInfo& View = Views[ViewIndex];
		View.ViewRect.Min = {0, 0};
		View.ViewRect.Max.X = View.ViewRect.Min.X + (int)View.Resolution.x;
		View.ViewRect.Max.Y = View.ViewRect.Min.Y + (int)View.Resolution.y;
	}
}

void SceneRenderer::ComputeVisibility()
{
	for (uint32 ViewIndex = 0; ViewIndex < Views.Size(); ++ViewIndex)
	{
		WViewInfo& ViewInfo = Views[ViewIndex];

		ViewInfo.PrimitiveVisibilityMap.Init(Scene->GetPrimitives().Size(), false);

		{
			const WEngine::WArray<PrimitiveInfo*>& Primitives = Scene->GetPrimitives();
			FrustumCulling(Primitives, ViewInfo);
			OcclusionCulling(Primitives, ViewInfo);
		}

	}

	GatherDynamicMeshElements(Views, ViewFamily);
}

void SceneRenderer::FrustumCulling(const WEngine::WArray<PrimitiveInfo*>& Primitives, WViewInfo& ViewInfo)
{
	WEngine::WArray<glm::vec3> Frustum(8);
	ViewInfo.ComputeFrustum(Frustum);

	WEngine::WTaskGraph::Get()->ParallelFor(Primitives.Size(), [this, &ViewInfo, &Primitives, &Frustum](uint32 index)
		{
			PrimitiveInfo* info = Primitives[index];
			const BoundingBox& box = info->Proxy->GetBoundingBox();

			if ((bUseBoxTest ? IsBoxInFrustum(Frustum, box.BoxMin, box.BoxMax) : true) && (bUseSphereTest ? IsSphereInFrustum(Frustum, glm::vec3(), 0.0f) : true))
			{
				ViewInfo.PrimitiveVisibilityMap[index] = true;
			}
		});
}

void SceneRenderer::OcclusionCulling(const WEngine::WArray<PrimitiveInfo*>& Primitives, WViewInfo& ViewInfo)
{
}

void SceneRenderer::GatherDynamicMeshElements(const WEngine::WArray<WViewInfo>& Views, const WSceneViewFamily *InFamily)
{
	const WEngine::WArray<PrimitiveInfo*>& Primitives = Scene->GetPrimitives();
	const WEngine::WArray<uint8>& PrimitiveMasks = Scene->GetPrimitiveMasks();
	uint32 NumPrimitive = Primitives.Size();

	for (uint32 PrimitiveIndex = 0; PrimitiveIndex < NumPrimitive; ++PrimitiveIndex)
	{
		if ((PrimitiveMasks[PrimitiveIndex] & (uint8)PrimitiveInfo::PrimitiveMask::Dynamic) > 0)
		{
			Primitives[PrimitiveIndex]->Proxy->GetDynamicMeshElements(InFamily->Views, Collector);
		}
	}
}

SceneRenderer* SceneRenderer::CreateRenderer(const WSceneViewFamily* InViewFamily)
{
	EShadingPath ShadingPath = InViewFamily->Scene->GetShadingPath();

	switch (ShadingPath)
	{
	case EShadingPath::Forward:
		break;
	case EShadingPath::Deferred:
		return new DeferredRenderer(InViewFamily);
	case EShadingPath::ForwardPlus:
		break;
	default:
		break;
	}
	return nullptr;
}

WViewInfo::WViewInfo(const WSceneViewInfo& SceneViewInfo)
	: WSceneViewInfo(SceneViewInfo)
{
	ViewRect = {0, 0, 0, 0};
}

void WViewInfo::SetupViewParameters(SceneViewUniformBufferParameters& Parameters)
{
	Parameters.ViewMatrix = ViewMatrices.ViewMatrix;
	Parameters.ProjectionMatrix = ViewMatrices.ProjectionMatrix;
	Parameters.ViewProjectionMatrix = ViewMatrices.ViewProjectionMatrix;
	Parameters.ViewLocation = ViewLocation;
}

void WViewInfo::CreateUniformBuffer(SceneViewUniformBufferParameters& Parameters)
{
	ViewUniformBuffer = SceneViewUniformBufferParameters::CreateUniformBuffer((uint8*)&Parameters, sizeof(Parameters), 1, EBufferUsageFlags::BF_UniformBuffer);
}

void WViewInfo::InitRHIResources()
{
	SceneViewUniformBufferParameters Parameters;

	SetupViewParameters(Parameters);

	CreateUniformBuffer(Parameters);
}