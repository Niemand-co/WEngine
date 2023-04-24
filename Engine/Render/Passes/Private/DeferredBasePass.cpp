#include "pch.h"
#include "Render/Passes/Public/DeferredBasePass.h"
#include "Render/Renderer/Public/DeferredRenderer.h"
#include "Render/Public/Buffer.h"
#include "Render/Public/RenderTarget.h"
#include "Render/Public/RenderDependencyGraph.h"
#include "Scene/Components/Public/MaterialComponent.h"

BEGIN_SHADER_PARAMETERS_STRUCT(DeferredBasePassParameters)
	SHADER_PARAMETER_TEXTURE(Texture2D, sa)
	RENDER_TARGET_SLOTS()
END_SHADER_PARAMETERS_STRUCT

void DeferredRenderer::RenderBasePass(WRDGBuilder& GraphBuilder, WViewInfo& View)
{
	glm::vec2 ViewRect = View.ViewMatrices.Rect;
	WRDGTexture* GBuffer0 = GraphBuilder.RegisterExternalTexture(View.Family->RenderTarget->GetHandle());

	DeferredBasePassParameters* Parameters = GraphBuilder.AllocateParameterStruct<DeferredBasePassParameters>();
	Parameters->RenderTarget.ColorTextures[0].Texture = GBuffer0;
	Parameters->sa = GetRenderCommandList()->CreateTexture(RHITextureDesc::CreateTexture2D(EFormat::A16R16G16B16_SFloat, {1, 0, 0, 0}, {1024, 1024}, 1, 1, ETextureCreateFlags::TextureCreate_SRV));

	GraphBuilder.AddPass("BasePass", Parameters, [&View, this](RHIRenderCommandList& CmdList, WRenderPassRHIRef RenderPass)
	{
		CmdList.SetViewport(View.ViewRect.Min.X, View.ViewRect.Min.Y, View.ViewRect.Max.X, View.ViewRect.Max.Y, 0.0f, 1.0f);
		CmdList.SetScissor(0, 0, View.ViewRect.Max.X - View.ViewRect.Min.X, View.ViewRect.Max.Y - View.ViewRect.Min.Y);
		WMeshPassProcessorRenderState RenderState;

		RenderState.SetBlendState(0, TStaticBlendStateRHI<true, EBlendOP::BlendAdd, EBlendFactor::FactorSrcAlpha, EBlendFactor::FactorOneMinusSrcAlpha>::GetRHI());
		RenderState.SetDepthStencilState(TStaticDepthStencilStateRHI<false, false>::GetRHI());
		RenderState.SetRasterizationState(TStaticRasterizationStateRHI<ECullMode::None>::GetRHI());
		RenderState.SetMultiSampleState(TStaticMultiSampleStateRHI<>::GetRHI());

		WDeferredBasePassMeshProcessor Processor(View.Family->Scene, &View, RenderState);

		WDynamicMeshPassDrawListContext DrawList;
		Processor.SetDrawListContext(&DrawList);

		const WEngine::WArray<WMeshBatch>& BatchElements = Collector.GetBatches();
		for (uint32 MeshIndex = 0; MeshIndex < BatchElements.Size(); ++MeshIndex)
		{
			Processor.AddMeshBatch(BatchElements[MeshIndex]);
		}
		DrawList.FinalizeCommand(CmdList, RenderPass);
	});
}

WDeferredBasePassVS::WDeferredBasePassVS(WVertexShaderRHIRef InVertexShaderRHI)
	: VertexShaderRHI(InVertexShaderRHI)
{
	//UniformBuffer = new WUniformBuffer(&Parameters.GetStructMetaData()->GetLayout());
}

WDeferredBasePassVS::~WDeferredBasePassVS()
{
}

void WDeferredBasePassVS::GetParametersBinding(const WViewInfo *View, const MaterialProxy* Material, WMeshDrawShaderBindings& Bindings)
{
	WMaterialShader::GetParametersBinding(View, Material, Bindings);
	View->SetupViewParameters(Parameters.View);
}

WDeferredBasePassPS::WDeferredBasePassPS(WPixelShaderRHIRef InPixelShaderRHI)
	: PixelShaderRHI(InPixelShaderRHI)
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

	BuildMeshDrawCommand(MeshBatch, RenderState, &BasePassShaders, Material, EPassFeature::PositionOnly);
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
	VertexShader = new WDeferredBasePassVS((WVertexShaderRHIRef)WShaderLibrary::GetShader("DebugDrawVert"));
	PixelShader = new WDeferredBasePassPS((WPixelShaderRHIRef)WShaderLibrary::GetShader("DebugDrawFrag"));
}