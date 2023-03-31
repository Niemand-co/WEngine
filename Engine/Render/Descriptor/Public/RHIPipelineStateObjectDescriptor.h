#pragma once
#include "RHI/Public/RHIBlendState.h"
#include "RHI/Public/RHIDepthStencilState.h"
#include "RHI/Public/RHIRasterizationState.h"

class RHIGraphicsPipelineStateDescriptor
{
public:

	RHIGraphicsPipelineStateDescriptor() = default;

	~RHIGraphicsPipelineStateDescriptor() = default;

public:

	uint8 RenderTargetCount = 0;

	class WShader* Shaders[MaxGraphicsPipelineShaderNum] = {};

	WBlendStateRHIRef BlendStates[MaxSimultaneousRenderTargets] = {};

	WVertexInputStateRHIRef VertexInputState;

	WDepthStencilStateRHIRef DepthStencilState;

	WRasterizationStateRHIRef RasterizationState;

	WMultiSampleStateRHIRef MultiSampleState;

	WRenderPassRHIRef RenderPass;



};

class RHIComputePipelineStateDescriptor
{
public:

	RHIComputePipelineStateDescriptor() = default;

	~RHIComputePipelineStateDescriptor() = default;

public:

	

};