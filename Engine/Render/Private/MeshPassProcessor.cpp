#include "pch.h"
#include "Render/Public/MeshPassProcessor.h"
#include "Render/Public/MeshPassProcessor.inl"
#include "Render/Mesh/Public/MeshBatch.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Public/Scene.h"
#include "Render/Public/SceneView.h"
#include "Render/Public/Buffer.h"
#include "Render/Public/Shader.h"
#include "Render/Descriptor/Public/RHIPipelineStateObjectDescriptor.h"

void WMeshDrawShaderBindings::Initialize(const WMeshPassProcessorShaderBase* Shaders)
{
	HashCode = WEngine::MemCrc32(Bindings.GetData(), sizeof(ShaderBindingSlot) * Bindings.Size());
}

void WMeshDrawShaderBindings::Add(EUniformBaseType InType, uint32 InCount)
{
	Bindings.Push(ShaderBindingSlot(Bindings.Size(), InCount, InType));
}

void WMeshDrawCommand::SetParameters(const WMeshBatch& MeshBatch, uint32 MeshBatchElementIndex, VertexInputStream& Stream, const WMeshPassProcessorShaderBase* Shaders, const RHIGraphicsPipelineStateDescriptor& InPipelineDescriptor)
{
	const WMeshBatchElement& Element = MeshBatch.Elements[MeshBatchElementIndex];

	VertexStream = Stream;

	IndexBuffer = Element.IndexBuffer ? Element.IndexBuffer->GetRHI() : nullptr;
	FirstIndex = Element.FirstIndex;
	NumPrimitives = Element.NumPrimitives;
	NumInstances = Element.NumInstances;

	PipelineDescriptor = InPipelineDescriptor;

	for (uint32 ShaderStage = 0; ShaderStage < MaxGraphicsPipelineShaderNum; ++ShaderStage)
	{
		ShaderBindings[ShaderStage].Initialize(Shaders);
	}
}

WMeshDrawShaderBindings& WMeshDrawCommand::GetShaderBinding(uint32 ShaderStage)
{
	RE_ASSERT(ShaderStage < MaxGraphicsPipelineShaderNum, "Out of shader stage range.");
	return ShaderBindings[ShaderStage];
}

bool WMeshDrawCommand::SubmitDrawBegin(WRenderPassRHIRef RenderPass, RHIRenderCommandList& CmdList)
{
	PipelineDescriptor.RenderPass = RenderPass;
	PipelineDescriptor.ShaderBindings = ShaderBindings;

	CmdList.SetViewport(0, 0, 1920, 1080, 0.0f, 1.0f);
	CmdList.SetScissor(0, 0, 1920, 1080);
	//CmdList.SetGraphicsPipelineState(&PipelineDescriptor);

	//CmdList.SetStreamResource(VertexStream);

	return true;
}

void WMeshDrawCommand::SubmitDrawEnd(RHIRenderCommandList& CmdList)
{
	//CmdList.BindIndexBuffer(IndexBuffer);
	//CmdList.DrawIndexedPrimitive(NumPrimitives * 3, FirstIndex, NumInstances);

}

WMeshDrawCommand& WDynamicMeshPassDrawListContext::AddCommand()
{
	return MeshCommands.AddInitialized();
}

void WDynamicMeshPassDrawListContext::FinalizeCommand(RHIRenderCommandList& CmdList, WRenderPassRHIRef RenderPass)
{
	for (WMeshDrawCommand& Command : MeshCommands)
	{
		WMeshDrawCommand::SubmitDrawCommand(Command, RenderPass, CmdList);
	}
}
