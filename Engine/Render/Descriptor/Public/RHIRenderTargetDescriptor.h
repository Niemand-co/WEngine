#pragma once

class RHITextureView;
class RHIRenderPass;

class RHIRenderTargetDescriptor
{
public:

	RHIRenderTargetDescriptor();

	virtual ~RHIRenderTargetDescriptor();

public:

	unsigned int width;

	unsigned int height;

	RHIRenderPass *renderPass;

	unsigned int bufferCount;

	RHITextureView **pBufferView;

};