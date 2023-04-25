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

void WMeshDrawCommand::SetParametersAndFinalize(const WMeshBatch& MeshBatch, uint32 MeshBatchElementIndex, const WMeshPassProcessorShaderBase* Shaders, const GraphicsPipelineStateId& InPipelineId)
{
	const WMeshBatchElement& Element = MeshBatch.Elements[MeshBatchElementIndex];

	VertexStream = Stream;

	IndexBuffer = Element.IndexBuffer ? Element.IndexBuffer->GetRHI() : nullptr;
	FirstIndex = Element.FirstIndex;
	NumPrimitives = Element.NumPrimitives;
	NumInstances = Element.NumInstances;

	Finalize(InPipelineId, Shaders);
}

void WMeshDrawCommand::Finalize(GraphicsPipelineStateId InPipelineId, const WMeshPassProcessorShaderBase* Shaders)
{
	PipelineId = InPipelineId;
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

bool WMeshDrawCommand::SubmitDrawBegin(const GraphicsPipelineStateSet& PipelineStateSet, RHIRenderCommandList& CmdList)
{
	RHIGraphicsPipelineStateInitializer PipelineState = PipelineStateSet.GetPipelineState(PipelineId);

	CmdList.SetGraphicsPipelineState(PipelineState);

	CmdList.SetStreamResource(VertexStream);

	return true;
}

void WMeshDrawCommand::SubmitDrawEnd(RHIRenderCommandList& CmdList)
{
	CmdList.BindIndexBuffer(IndexBuffer);
	CmdList.DrawIndexedPrimitive(NumPrimitives * 3, FirstIndex, NumInstances);
}

WMeshDrawCommand& WDynamicMeshPassDrawListContext::AddCommand()
{
	return MeshCommands.AddInitialized();
}

void WDynamicMeshPassDrawListContext::FinalizeCommand(
	const WMeshBatch& MeshBatch,
	uint32 BatchElementIndex,
	const WMeshPassProcessorShaderBase* Shaders,
	const RHIGraphicsPipelineStateInitializer& PipelineState,
	WMeshDrawCommand& MeshDrawCommand)
{
	GraphicsPipelineStateId PipelineId = PipelineStateSet.FindOrAdd(PipelineState);

	MeshDrawCommand.SetParametersAndFinalize(MeshBatch, BatchElementIndex, Shaders, PipelineId);
}