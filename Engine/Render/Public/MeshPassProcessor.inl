#pragma once

template<typename PassShaderType>
void WMeshPassProcessor::BuildMeshDrawCommand(const WMeshBatch& MeshBatch,
											  const WMeshPassProcessorRenderState& RenderState,
											  PassShaderType *PassShader,
											  const MaterialProxy& Material,
											  EPassFeature Feature)
{
	VertexInputStream VertexStream;

	RHIGraphicsPipelineStateInitializer Initializer = {};
	{
		
		for (uint32 BlendIndex = 0; BlendIndex < MaxSimultaneousRenderTargets; ++BlendIndex)
		{
			if(RenderState.BlendStates[BlendIndex] == nullptr)
				break;
			Initializer.BlendState->SetAttachmentBlendState(BlendIndex, RenderState.BlendStates[BlendIndex]);
			Initializer.RenderTargetFormats[BlendIndex] = RenderState.RenderTargets[BlendIndex].Format;
			Initializer.RenderTargetLoadOps[BlendIndex] = RenderState.RenderTargets[BlendIndex].LoadOp;
			Initializer.RenderTargetStoreOps[BlendIndex] = RenderState.RenderTargets[BlendIndex].StoreOp;
			Initializer.RenderTargetInitialLayouts[BlendIndex] = RenderState.RenderTargets[BlendIndex].InitialLayout;
			Initializer.RenderTargetEnabled++;
		}

		Initializer.DepthStencilFormat = RenderState.DepthStencil.Format;
		Initializer.DepthTargetLoadAction = RenderState.DepthStencil.LoadOp;
		Initializer.DepthTargetStoreAction = RenderState.DepthStencil.StoreOp;

		Initializer.DepthStencilState = RenderState.DepthStencilState;

		Initializer.RasterizationState = RenderState.RasterizationState;

		switch (Feature)
		{
		case EPassFeature::Default:
			Initializer.BoundShaderState.VertexInputState = MeshBatch.VertexFactory->GetVertexInput(EVertexInputType::Default);
			MeshBatch.VertexFactory->GetStreams(EVertexInputType::Default, VertexStream);
			break;
		case EPassFeature::PositionOnly:
			Initializer.BoundShaderState.VertexInputState = MeshBatch.VertexFactory->GetVertexInput(EVertexInputType::PositionOnly);
			MeshBatch.VertexFactory->GetStreams(EVertexInputType::PositionOnly, VertexStream);
			break;
		case EPassFeature::PositionAndNormal:
			Initializer.BoundShaderState.VertexInputState = MeshBatch.VertexFactory->GetVertexInput(EVertexInputType::PositionAndNormal);
			MeshBatch.VertexFactory->GetStreams(EVertexInputType::PositionAndNormal, VertexStream);
			break;
		default:
			RE_ASSERT(false, "Error pass feature.");
		}

		Initializer.MultiSampleState = RenderState.MultiSampleState;

		Initializer.BoundShaderState.VertexShaderRHI = PassShader->GetVertexShaderRHI();
		Initializer.BoundShaderState.GeometryShaderRHI = PassShader->GetGeometryShaderRHI();
		Initializer.BoundShaderState.PixelShaderRHI = PassShader->GetPixelShaderRHI();

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

		DrawList->FinalizeCommand(MeshBatch, ElementIndex, PassShader, Initializer, DrawCommand);
	}

}