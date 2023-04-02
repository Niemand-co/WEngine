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

void WMeshDrawCommand::SetParameters(const WMeshBatch& MeshBatch, uint32 MeshBatchElementIndex, const WMeshPassProcessorShaderBase* Shaders, const RHIGraphicsPipelineStateDescriptor& InPipelineDescriptor)
{
	const WMeshBatchElement& Element = MeshBatch.Elements[MeshBatchElementIndex];

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

void WMeshDrawCommand::SubmitDrawBegin(WRenderPassRHIRef RenderPass)
{
	PipelineDescriptor.RenderPass = RenderPass;
	PipelineDescriptor.ShaderBindings = ShaderBindings;

	GetRenderCommandList()->SetGraphicsPipelineState(&PipelineDescriptor);
}

void WMeshDrawCommand::SubmitDrawEnd()
{
	GetRenderCommandList()->DrawIndexedPrimitive(NumPrimitives * 3, FirstIndex, NumInstances);
}
