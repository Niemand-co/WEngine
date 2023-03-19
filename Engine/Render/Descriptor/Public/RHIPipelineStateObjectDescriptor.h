#pragma once
#include "RHI/Public/RHIBlendState.h"
#include "RHI/Public/RHIDepthStencilState.h"
#include "RHI/Public/RHIRasterizationState.h"

#define MaxGraphicsPipelineShaderNum 3

class RHIGraphicsPipelineStateDescriptor
{
public:

	RHIGraphicsPipelineStateDescriptor() = default;

	~RHIGraphicsPipelineStateDescriptor() = default;

public:

	uint8 RenderTargetCount = 0;

	class WShader* Shaders[MaxGraphicsPipelineShaderNum] = {};

	WBlendStateRHIRef BlendStates[MaxSimultaneousRenderTargets] = {};

	RHIVertexInputDescriptor VertexInputAttrib;

	WDepthStencilStateRHIRef DepthStencilState;

	WRasterizationStateRHIRef RasterizationState;

	WMultiSampleStateRHIRef MultiSampleState;

	RHIPipelineResourceLayoutDescriptor DescriptorSetLayoutInfo;

	WRenderPassRHIRef RenderPass;

};

class RHIComputePipelineStateDescriptor
{
public:

	RHIComputePipelineStateDescriptor() = default;

	~RHIComputePipelineStateDescriptor() = default;

public:

	

};