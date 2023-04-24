#include "pch.h"
#include "Render/Renderer/Public/DeferredRenderer.h"
#include "Render/Public/Scene.h"
#include "Render/Public/Shader.h"
#include "Scene/Components/Public/PrimitiveComponent.h"
#include "Scene/Components/Public/DirectionalLightComponent.h"
#include "Render/Public/RenderDependencyGraph.h"
#include "Render/Public/RenderDependencyGraphResource.h"
#include "Render/Public/RenderDependencyGraphParameter.h"
#include "Render/Public/Viewport.h"
#include "Render/Descriptor/Public/RHIPipelineStateObjectDescriptor.h"
#include "RHI/Public/RHIViewport.h"

WRDGBuilder* GetRDGBuilder()
{
	static WRDGBuilder *Builder = new WRDGBuilder;
	return Builder;
}

DeferredRenderer::DeferredRenderer(const WSceneViewFamily* InViewFamily)
	: SceneRenderer(InViewFamily)
{
}

DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::Render(WRDGBuilder& GraphBuilder)
{
	PrepareViewForRendering();

	InitView();

	for (uint32 ViewIndex = 0; ViewIndex < Views.Size(); ++ViewIndex)
	{
		WViewInfo& View = Views[ViewIndex];

		RenderPrePass(GraphBuilder, View);

		RenderBasePass(GraphBuilder, View);

		RenderShadowPass(GraphBuilder, View);

		RenderLightPass(GraphBuilder, View);

		RenderSkybox(GraphBuilder, View);

		RenderTranslucent(GraphBuilder, View);

		RenderPostEffect(GraphBuilder, View);

	}
}

void DeferredRenderer::RenderPrePass(WRDGBuilder& GraphBuilder, WViewInfo& View)
{

}

void DeferredRenderer::RenderShadowPass(WRDGBuilder& GraphBuilder, WViewInfo& View)
{
}

void DeferredRenderer::RenderLightPass(WRDGBuilder& GraphBuilder, WViewInfo& View)
{
	RE_LOG("Render Light Pass");
}

void DeferredRenderer::RenderSkybox(WRDGBuilder& GraphBuilder, WViewInfo& View)
{
}

void DeferredRenderer::RenderTranslucent(WRDGBuilder& GraphBuilder, WViewInfo& View)
{
}

void DeferredRenderer::RenderPostEffect(WRDGBuilder& GraphBuilder, WViewInfo& View)
{
}
