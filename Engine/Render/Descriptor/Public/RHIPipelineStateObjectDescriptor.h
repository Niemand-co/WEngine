#pragma once
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"
#include "Render/Descriptor/Public/RHIBlendDescriptor.h"
#include "Render/Descriptor/Public/RHIDepthStencilDescriptor.h"
#include "Render/Descriptor/Public/RHIPipelineResourceLayoutDescriptor.h"
#include "Render/Descriptor/Public/RHIRasterizationStateDescriptor.h"

#define MaxGraphicsPipelineShaderNum 3

class RHIGraphicsPipelineStateDescriptor
{
public:

	RHIGraphicsPipelineStateDescriptor() = default;

	~RHIGraphicsPipelineStateDescriptor() = default;

public:

	RHIVertexInputDescriptor VertexInputAttrib;

	RHIBlendDescriptor BlendState;

	RHIDepthStencilDescriptor DepthStencilState;

	class WShader* Shaders[MaxGraphicsPipelineShaderNum] = {};

	RHIPipelineResourceLayoutDescriptor DescriptorSetLayoutInfo;

	RHIRasterizationStateDescriptor RasterizationState;

};

class RHIComputePipelineStateDescriptor
{
public:

	RHIComputePipelineStateDescriptor() = default;

	~RHIComputePipelineStateDescriptor() = default;

public:

	

};