#include "pch.h"
#include "Render/Passes/Public/DeferredBasePass.h"
#include "Render/Renderer/Public/DeferredRenderer.h"
#include "Render/Public/Buffer.h"
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

	GraphBuilder->AddPass("BasePass", Parameters, [&View, this](RHIRenderCommandList& CmdList, WRenderPassRHIRef RenderPass)
	{
		CmdList.SetViewport(View.ViewRect.Min.X, View.ViewRect.Min.Y, View.ViewRect.Max.X, View.ViewRect.Max.Y, 0.0f, 1.0f);

		WMeshPassProcessorRenderState RenderState;

		RenderState.SetBlendState(0, TStaticBlendStateRHI<false>::GetRHI());
		RenderState.SetDepthStencilState(TStaticDepthStencilStateRHI<true, true, ECompareOP::Greater>::GetRHI());
		RenderState.SetRasterizationState(TStaticRasterizationStateRHI<ECullMode::Back>::GetRHI());
		RenderState.SetMultiSampleState(TStaticMultiSampleStateRHI<>::GetRHI());

		WDeferredBasePassMeshProcessor Processor(View.Family->Scene, &View, RenderState);
		for (uint32 MeshIndex = 0; MeshIndex < Batches.Size(); ++MeshIndex)
		{
			Processor.AddMeshBatch(*Batches[MeshIndex]);
		}
	});
}

WDeferredBasePassVS::WDeferredBasePassVS()
{
	UniformBuffer = new WUniformBuffer(&Parameters.GetStructMetaData()->GetLayout());
}

WDeferredBasePassVS::~WDeferredBasePassVS()
{
}

void WDeferredBasePassVS::GetParametersBinding(const WViewInfo *View, const MaterialProxy* Material, WMeshDrawShaderBindings& Bindings)
{
	WMaterialShader::GetParametersBinding(View, Material, Bindings);
	View->SetupViewParameters(Parameters.View);
}

WDeferredBasePassPS::WDeferredBasePassPS()
{
}

WDeferredBasePassPS::~WDeferredBasePassPS()
{
}

void WDeferredBasePassPS::GetParametersBinding(const WViewInfo* View, const MaterialProxy* Material, WMeshDrawShaderBindings& Bindings)
{
	WMaterialShader::GetParametersBinding(View, Material, Bindings);
	Parameters.Color = glm::vec3(1, 0, 0);
	const ShaderParametersLayout& Layout = Parameters.GetStructMetaData()->GetLayout();

	{
		const WEngine::WArray<ShaderParametersLayout::ResourceInfo>& Uniforms = Layout.GetUniforms();
		for (auto& Info : Uniforms)
		{
			Bindings.Add(Info.Type, 1);
		}
	}
	const WEngine::WArray<ShaderParametersLayout::ResourceInfo>& Uniforms = Layout.GetTextures();


}

WDeferredBasePassMeshProcessor::WDeferredBasePassMeshProcessor(const RScene* InScene, const WViewInfo* InView, const WMeshPassProcessorRenderState& InRenderState)
	: WMeshPassProcessor(InScene, InView),
	  RenderState(InRenderState)
{
}

void WDeferredBasePassMeshProcessor::AddMeshBatch(const WMeshBatch& MeshBatch)
{
	MaterialProxy& Material = *MeshBatch.Material;
	if (Scene->GetShadingPath() == EShadingPath::Forward)
	{
		ProcessForwardShadingPath(MeshBatch, Material);
	}
	else if (Scene->GetShadingPath() == EShadingPath::Deferred)
	{
		ProcessDeferredShadingPath(MeshBatch, Material);
	}
	else if (Scene->GetShadingPath() == EShadingPath::ForwardPlus)
	{
		ProcessForwardPlusShadingPath(MeshBatch, Material);
	}
}

bool WDeferredBasePassMeshProcessor::ProcessDeferredShadingPath(const WMeshBatch& MeshBatch, const MaterialProxy& Material)
{
	WMeshPassProcessorShader<WDeferredBasePassVS, WDummyMaterialShader, WDeferredBasePassPS> BasePassShaders;
	GetBasePassShaders(BasePassShaders.VertexShader, BasePassShaders.PixelShader);

	BuildMeshDrawCommand(MeshBatch, RenderState, &BasePassShaders, Material, EPassFeature::PositionAndNormal);
	return true;
}

bool WDeferredBasePassMeshProcessor::ProcessForwardShadingPath(const WMeshBatch& MeshBatch, const MaterialProxy& Material)
{
	return false;
}

bool WDeferredBasePassMeshProcessor::ProcessForwardPlusShadingPath(const WMeshBatch& MeshBatch, const MaterialProxy& Material)
{
	return false;
}

void GetBasePassShaders(WDeferredBasePassVS*& VertexShader, WDeferredBasePassPS*& PixelShader)
{
	VertexShader = new WDeferredBasePassVS();
	PixelShader = new WDeferredBasePassPS();
}