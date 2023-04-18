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

void DeferredRenderer::Render()
{
	PrepareViewForRendering();

	InitView();

	for (uint32 ViewIndex = 0; ViewIndex < Views.Size(); ++ViewIndex)
	{
		WViewInfo& View = Views[ViewIndex];

		RenderPrePass(View);

		RenderBasePass(View);

		RenderShadowPass(View);

		RenderLightPass(View);

		RenderSkybox(View);

		RenderTranslucent(View);

		RenderPostEffect(View);

	}
	GetRDGBuilder()->Execute();
}

void DeferredRenderer::RenderPrePass(WViewInfo& View)
{

}

void DeferredRenderer::RenderShadowPass(WViewInfo& View)
{
}

void DeferredRenderer::RenderLightPass(WViewInfo& View)
{
	RE_LOG("Render Light Pass");
}

void DeferredRenderer::RenderSkybox(WViewInfo& View)
{
}

void DeferredRenderer::RenderTranslucent(WViewInfo& View)
{
}

void DeferredRenderer::RenderPostEffect(WViewInfo& View)
{
}
