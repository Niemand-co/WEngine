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

	ShaderBindings->Initialize(Shaders);
}

WMeshDrawShaderBindings& WMeshDrawCommand::GetShaderBinding(uint32 ShaderStage)
{
	RE_ASSERT(ShaderStage < MaxGraphicsPipelineShaderNum, "Out of shader stage range.");
	return ShaderBindings[ShaderStage];
}

bool WMeshDrawCommand::SubmitDrawBegin(WMeshDrawCommand& Command, WRenderPassRHIRef RenderPass, RHIRenderCommandList& CmdList)
{
	Command.PipelineDescriptor.RenderPass = RenderPass;
	Command.PipelineDescriptor.ShaderBindings = Command.ShaderBindings;

	CmdList.SetGraphicsPipelineState(&Command.PipelineDescriptor);

	CmdList.SetStreamResource(Command.VertexStream);

	return true;
}

void WMeshDrawCommand::SubmitDrawEnd(WMeshDrawCommand& Command, RHIRenderCommandList& CmdList)
{
	CmdList.BindIndexBuffer(Command.IndexBuffer);
	CmdList.DrawIndexedPrimitive(Command.NumPrimitives * 3, Command.FirstIndex, Command.NumInstances);
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
