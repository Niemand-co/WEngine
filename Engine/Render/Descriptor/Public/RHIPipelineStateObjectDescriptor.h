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

	~RHIPipelineStateObjectDescriptor() = default;

public:

	RHIVertexInputDescriptor *vertexDescriptor;

	RHIBlendDescriptor *blendDescriptor;

	RHIDepthStencilDescriptor *depthStencilDescriptor;

	RHIShader** pShader;

	unsigned int shaderCount;

	RHIRenderPass *renderPass;

	unsigned int subpass;

	RHIPipelineResourceLayout *pipelineResourceLayout;

};