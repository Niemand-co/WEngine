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

DeferredRenderer::DeferredRenderer(const WSceneViewFamily* InViewFamily)
	: SceneRenderer(InViewFamily)
{
	GraphBuilder = new WRDGBuilder();
}

DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::Render()
{
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
	GraphBuilder->Execute();
}

void DeferredRenderer::InitView()
{
	ComputeVisibility();
	
}

void DeferredRenderer::RenderPrePass(WViewInfo& View)
{

}

void DeferredRenderer::RenderBasePass(WViewInfo& View)
{
	glm::vec2 ViewRect = View.ViewMatrices.Rect;
	WRDGTexture * GBuffer0 = GraphBuilder->RegisterExternalTexture(View.Family->RenderTarget->GetHandle());

	const WRDGTextureDesc DepthDesc = WRDGTextureDesc::GetTexture2DDesc(Format::D16_Unorm, { (uint32)ViewRect.x, (uint32)ViewRect.y, 1u }, { 0.0f, 0.0f, 0.0f, 0.0f });
	WRDGTexture* DepthBuffer = GraphBuilder->CreateTexture(DepthDesc, "Depth");

	DeferredBasePassParameters* Parameters = GraphBuilder->AllocateParameterStruct<DeferredBasePassParameters>();
	Parameters->RenderTarget.ColorTextures[0].Texture = GBuffer0;
	Parameters->RenderTarget.DepthStencilTexture.Texture = DepthBuffer;

	GraphBuilder->AddPass("BasePass", Parameters, [](RHIRenderCommandList& CmdList)
		{
			RHIGraphicsPipelineStateDescriptor PSODescriptor = {};
			{
				PSODescriptor.Shaders[(uint8)ShaderStage::Vertex] = WShaderLibrary::GetShader("OpaqueVert");
				PSODescriptor.Shaders[(uint8)ShaderStage::Pixel] = WShaderLibrary::GetShader("OpaqueFrag");


			}
			CmdList.SetGraphicsPipelineState(&PSODescriptor);
			CmdList.DrawIndexedPrimitive(3, 0, 1);
		});
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
