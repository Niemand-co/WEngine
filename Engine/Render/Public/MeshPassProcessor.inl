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
		uint32 BlendIndex = 0;
		for (; BlendIndex < MaxSimultaneousRenderTargets; ++BlendIndex)
		{
			if(RenderState.BlendStates[BlendIndex] == nullptr)
				break;
			PipelineInfo.BlendStates[BlendIndex] = RenderState.BlendStates[BlendIndex];
		}

		PipelineInfo.RenderTargetCount = BlendIndex;

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

		PipelineInfo.Shaders[(uint8)EShaderStage::Vertex] = PassShader->GetVertexShader();
		PipelineInfo.Shaders[(uint8)EShaderStage::Geometry] = PassShader->GetGeometryShader();
		PipelineInfo.Shaders[(uint8)EShaderStage::Pixel] = PassShader->GetPixelShader();
	}

	for (uint32 ElementIndex = 0; ElementIndex < MeshBatch.Elements.Size(); ++ElementIndex)
	{
		WMeshDrawCommand& DrawCommand = DrawList->AddCommand();
		if (PassShader->GetVertexShader())
		{
			WMeshDrawShaderBindings& ShaderBindings = DrawCommand.GetShaderBinding((uint32)EShaderStage::Vertex);
			PassShader->GetVertexShader()->GetParametersBinding(View, &Material, ShaderBindings);
		}

		if (PassShader->GetGeometryShader())
		{
			WMeshDrawShaderBindings& ShaderBindings = DrawCommand.GetShaderBinding((uint32)EShaderStage::Geometry);
			PassShader->GetGeometryShader()->GetParametersBinding(View, &Material, ShaderBindings);
		}

		if (PassShader->GetPixelShader())
		{
			WMeshDrawShaderBindings& ShaderBindings = DrawCommand.GetShaderBinding((uint32)EShaderStage::Pixel);
			PassShader->GetPixelShader()->GetParametersBinding(View, &Material, ShaderBindings);
		}

		DrawCommand.SetParameters(MeshBatch, ElementIndex, PassShader, PipelineInfo);
	}

}