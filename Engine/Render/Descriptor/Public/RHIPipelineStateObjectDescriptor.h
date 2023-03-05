#pragma once
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"
#include "Render/Descriptor/Public/RHIBlendDescriptor.h"
#include "Render/Descriptor/Public/RHIDepthStencilDescriptor.h"
#include "Render/Descriptor/Public/RHIPipelineResourceLayoutDescriptor.h"
#include "Render/Descriptor/Public/RHIRasterizationStateDescriptor.h"

class RHIPipelineStateObjectDescriptor
{
public:

	RHIPipelineStateObjectDescriptor() = default;

	~RHIPipelineStateObjectDescriptor() = default;

public:

	RHIVertexInputDescriptor VertexInputAttrib;

	RHIBlendDescriptor BlendState;

	RHIDepthStencilDescriptor DepthStencilState;

	class WShader* pShader = nullptr;

	unsigned int shaderCount = 0;

	RHIPipelineResourceLayoutDescriptor DescriptorSetLayout;

	RHIRasterizationStateDescriptor RasterizationState;

};