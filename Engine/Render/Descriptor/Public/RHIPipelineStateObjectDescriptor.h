#pragma once

class RHIVertexInputDescriptor;
class RHIBlendDescriptor;
class RHIDepthStencilDescriptor;
class RHIShader;
class RHIRenderPass;
class RHIPipelineResourceLayout;
class RHIRasterizationStateDescriptor;
class RHIScissor;
class RHIViewport;

class RHIPipelineStateObjectDescriptor
{
public:

	RHIPipelineStateObjectDescriptor() = default;

	~RHIPipelineStateObjectDescriptor() = default;

public:

	RHIVertexInputDescriptor *vertexDescriptor = nullptr;

	RHIBlendDescriptor *blendDescriptor = nullptr;

	RHIDepthStencilDescriptor *depthStencilDescriptor = nullptr;

	RHIShader** pShader = nullptr;

	unsigned int shaderCount = 0;

	RHIRenderPass *renderPass = nullptr;

	unsigned int subpass = 0;

	RHIPipelineResourceLayout *pipelineResourceLayout = nullptr;

	RHIRasterizationStateDescriptor *rasterizationStateDescriptor = nullptr;

	unsigned int scissorCount = 0;

	RHIScissor *pScissors = nullptr;

	unsigned int viewportCount = 0;

	RHIViewport *pViewports = nullptr;

};