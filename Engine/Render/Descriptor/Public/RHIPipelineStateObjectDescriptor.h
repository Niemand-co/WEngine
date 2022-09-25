#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHIVertexInputDescriptor;
class RHIBlendDescriptor;
class RHIDepthStencilDescriptor;
class RHIShader;
class RHIRenderPass;

class RHIPipelineStateObjectDescriptor : public RHIDescriptor
{
public:

	RHIPipelineStateObjectDescriptor();

	virtual ~RHIPipelineStateObjectDescriptor();

public:

	RHIVertexInputDescriptor *vertexDescriptor;

	RHIBlendDescriptor *blendDescriptor;

	RHIDepthStencilDescriptor *depthStencilDescriptor;

	RHIShader** pShader;

	unsigned int shaderCount;

	RHIRenderPass *renderPass;

};