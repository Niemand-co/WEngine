#pragma once

template<typename PassShaderType>
void WMeshPassProcessor::BuildMeshDrawCommand(const WMeshBatch& MeshBatch,
											  const WMeshPassProcessorRenderState& RenderState,
											  PassShaderType *PassShader,
											  const MaterialProxy& Material,
											  EPassFeature Feature)
{
	RHIGraphicsPipelineStateDescriptor PipelineInfo = {};
	{
		for (uint32 BlendIndex = 0; BlendIndex < RenderState.RenderTargetCount; ++BlendIndex)
		{
			PipelineInfo.BlendStates[BlendIndex] = RenderState.BlendStates[BlendIndex];
		}

		PipelineInfo.RenderTargetCount = RenderState.RenderTargetCount;

		PipelineInfo.DepthStencilState = RenderState.DepthStencilState;

		PipelineInfo.RasterizationState = RenderState.RasterizationState;

		switch (Feature)
		{
		case EPassFeature::Default:
			PipelineInfo.VertexInputState = MeshBatch.VertexFactory->GetVertexInput(EVertexInputType::Default);
			break;
		case EPassFeature::PositionOnly:
			PipelineInfo.VertexInputState = MeshBatch.VertexFactory->GetVertexInput(EVertexInputType::PositionOnly);
			break;
		case EPassFeature::PositionAndNormal:
			PipelineInfo.VertexInputState = MeshBatch.VertexFactory->GetVertexInput(EVertexInputType::PositionAndNormal);
			break;
		default:
			RE_ASSERT(false, "Error pass feature.");
		}

		PipelineInfo.MultiSampleState = RenderState.MultiSampleState;

		PipelineInfo.Shaders[EShaderStage::Vertex] = PassShader->GetVertexShaderRHI();
		PipelineInfo.Shaders[EShaderStage::Geometry] = PassShader->GetPixelShaderRHI();
		PipelineInfo.Shaders[EShaderStage::Pixel] = PassShader->GetVertexShaderRHI();
	}

	WMeshDrawCommand DrawCommand;
	if (PassShader->GetVertexShader())
	{
		ShaderBindingSlot& Slot = DrawCommand.GetShaderBinding(EShaderStage::Vertex);
		PassShader->GetVertexShader()->GetParametersBinding(View, Material);
	}

	if (PassShader->GetGeometryShader())
	{
		ShaderBindingSlot& Slot = DrawCommand.GetShaderBinding(EShaderStage::Geometry);
		PassShader->GetGeometryShader()->GetParametersBinding(View, Material);
	}

	if (PassShader->GetPixelShader())
	{
		ShaderBindingSlot& Slot = DrawCommand.GetShaderBinding(EShaderStage::Pixel);
		PassShader->GetPixelShader()->GetParametersBinding(View, Material);
	}

	for (uint32 ElementIndex = 0; ElementIndex < MeshBatch.Elements.Size(); ++ElementIndex)
	{
		DrawCommand.SetParameters(MeshBatch, ElementIndex, PassShader, PipelineInfo);
	}

}