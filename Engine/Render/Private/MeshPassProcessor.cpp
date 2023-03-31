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
	WMaterialShader *VertexShader = Shaders->GetVertexShader();
	if (VertexShader)
	{
		ShaderBindingSlot& Slot = Slots[(uint8)EShaderStage::Vertex];
		
	}
}

void WMeshDrawCommand::SetParameters(const WMeshBatch& MeshBatch, uint32 MeshBatchElementIndex, const WMeshPassProcessorShaderBase* Shaders, const RHIGraphicsPipelineStateDescriptor& InPipelineDescriptor)
{
	const WMeshBatchElement& Element = MeshBatch.Elements[MeshBatchElementIndex];

	IndexBuffer = Element.IndexBuffer ? Element.IndexBuffer->GetRHI() : nullptr;
	FirstIndex = Element.FirstIndex;
	NumPrimitives = Element.NumPrimitives; 
	NumInstances = Element.NumInstances;

	PipelineDescriptor = InPipelineDescriptor;

	ShaderBindings.Initialize(Shaders);
}

void WMeshDrawCommand::SubmitDrawBegin(WRenderPassRHIRef RenderPass)
{
	PipelineDescriptor.RenderPass = RenderPass;

	GetRenderCommandList()->SetGraphicsPipelineState(&PipelineDescriptor);
}

void WMeshDrawCommand::SubmitDrawEnd()
{
	GetRenderCommandList()->DrawIndexedPrimitive(NumPrimitives * 3, FirstIndex, NumInstances);
}
