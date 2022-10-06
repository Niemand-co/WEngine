#pragma once

class RHIVertexInputDescriptor;
class RHIBlendDescriptor;
class RHIDepthStencilDescriptor;
class RHIShader;
class RHIRenderPass;
class RHIPipelineResourceLayout;

class RHIPipelineStateObjectDescriptor
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

	RHIPipelineResourceLayout *pipelineResourceLayout;

};