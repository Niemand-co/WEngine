#include "pch.h"
#include "Render/Passes/Public/DeferredBasePass.h"
#include "Render/Renderer/Public/DeferredRenderer.h"
#include "Render/Public/SceneView.h"
#include "Render/Public/RenderTarget.h"
#include "Render/Public/RenderDependencyGraph.h"
#include "Scene/Components/Public/MaterialComponent.h"

void DeferredRenderer::RenderBasePass(WViewInfo& View)
{
	glm::vec2 ViewRect = View.ViewMatrices.Rect;
	WRDGTexture* GBuffer0 = GraphBuilder->RegisterExternalTexture(View.Family->RenderTarget->GetHandle());

	DeferredBasePassParameters* Parameters = GraphBuilder->AllocateParameterStruct<DeferredBasePassParameters>();
	Parameters->RenderTarget.ColorTextures[0].Texture = GBuffer0;

	DeferredBasePassParameters::GetStructMetaData()->GetLayout();

	WDeferredBasePassVS *Shader = new WDeferredBasePassVS();

	GraphBuilder->AddPass("BasePass", Parameters, [&View](RHIRenderCommandList& CmdList, WRenderPassRHIRef RenderPass)
	{
		CmdList.SetViewport(View.ViewRect.Min.X, View.ViewRect.Min.Y, View.ViewRect.Max.X, View.ViewRect.Max.Y, 0.0f, 1.0f);

		WMeshPassProcessorRenderState RenderState;

		RenderState.SetBlendState(0, TStaticBlendStateRHI<false>::GetRHI());
		RenderState.SetDepthStencilState(TStaticDepthStencilStateRHI<true, true, ECompareOP::Greater>::GetRHI());
		RenderState.SetRasterizationState(TStaticRasterizationStateRHI<ECullMode::Back>::GetRHI());
		RenderState.SetMultiSampleState(TStaticMultiSampleStateRHI<>::GetRHI());

		WDeferredBasePassMeshProcessor Processor(View.Family->Scene, &View, RenderState);
	});
}

WDeferredBasePassVS::WDeferredBasePassVS()
{
	
}

WDeferredBasePassVS::~WDeferredBasePassVS()
{
}

void WDeferredBasePassVS::GetParametersBinding(RScene* Scene, MaterialProxy* Material)
{
	
}

WDeferredBasePassMeshProcessor::WDeferredBasePassMeshProcessor(const RScene* InScene, const WViewInfo* InView, const WMeshPassProcessorRenderState& InRenderState)
	: WMeshPassProcessor(InScene, InView),
	  RenderState(InRenderState)
{
}
