#include "pch.h"
#include "Render/Public/MeshPassProcessor.h"
#include "Render/Mesh/Public/MeshBatch.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Render/Public/Scene.h"
#include "Render/Public/SceneView.h"
#include "Render/Descriptor/Public/RHIPipelineStateObjectDescriptor.h"

void WMeshPassProcessor::BuildMeshDrawCommand(const WMeshBatch MeshBatch, const WMeshPassProcessorRenderState& RenderState)
{
	RHIGraphicsPipelineStateDescriptor PipelineInfo = {};
	{
		PipelineInfo.BlendState = RenderState.GetBlendState();
		PipelineInfo.DepthStencilState = RenderState.GetDepthStencilState();
		PipelineInfo.RasterizationState = RenderState.GetRasterizationState();
		PipelineInfo.VertexInputAttrib = MeshBatch.VertexFactory->GetDeclaration();
	}


}