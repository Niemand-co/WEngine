#pragma once
#include "Render/Public/Shader.h"

class RHIGraphicsPipelineStateDescriptor
{
public:

	RHIGraphicsPipelineStateDescriptor() = default;

	~RHIGraphicsPipelineStateDescriptor() = default;

	void Finalize(uint32 LayoutHash)
	{
		WEngine::WArray<void*> Data(8 + RenderTargetCount);
		{ 
			Data[0] = Shaders[0] ? Shaders[0]->GetVertexShader() : nullptr;
			Data[1] = Shaders[1] ? Shaders[1]->GetGeometryShader() : nullptr;
			Data[2] = Shaders[2] ? Shaders[2]->GetPixelShader() : nullptr;
			Data[3] = VertexInputState;
			Data[4] = DepthStencilState;
			Data[5] = RasterizationState;
			Data[6] = MultiSampleState;
			Data[7] = RenderPass;
		}

		for (uint32 RenderTargetIndex = 0; RenderTargetIndex < RenderTargetCount; ++RenderTargetIndex)
		{
			Data[RenderTargetIndex + 8] = BlendStates[RenderTargetIndex];
		}

		HashCode = WEngine::MemCrc32(Data.GetData(), Data.Size() * 8, LayoutHash);
	}

	uint32 GetHashCode() const { return HashCode; }

public:

	uint8 RenderTargetCount = 0;

	class WShader* Shaders[MaxGraphicsPipelineShaderNum] = {0};

	WBlendStateRHIRef BlendStates[MaxSimultaneousRenderTargets] = {0};

	WVertexInputStateRHIRef VertexInputState = nullptr;

	WDepthStencilStateRHIRef DepthStencilState = nullptr;

	WRasterizationStateRHIRef RasterizationState = nullptr;

	WMultiSampleStateRHIRef MultiSampleState = nullptr;

	WRenderPassRHIRef RenderPass = nullptr;

	struct WMeshDrawShaderBindings *ShaderBindings = nullptr;

	uint32 HashCode = 0;

};

class RHIComputePipelineStateDescriptor
{
public:

	RHIComputePipelineStateDescriptor() = default;

	~RHIComputePipelineStateDescriptor() = default;

public:

	

};