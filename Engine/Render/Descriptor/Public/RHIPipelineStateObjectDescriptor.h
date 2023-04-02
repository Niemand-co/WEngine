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

	WVertexInputStateRHIRef VertexInputState = nullptr;

	WDepthStencilStateRHIRef DepthStencilState = nullptr;

	WRasterizationStateRHIRef RasterizationState = nullptr;

	WMultiSampleStateRHIRef MultiSampleState = nullptr;

	WRenderPassRHIRef RenderPass = nullptr;

	WMeshDrawShaderBindings *ShaderBindings = nullptr;

};

class RHIComputePipelineStateDescriptor
{
public:

	RHIComputePipelineStateDescriptor() = default;

	~RHIComputePipelineStateDescriptor() = default;

public:

	

};