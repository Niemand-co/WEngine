#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHITextureView;
class RHIRenderPass;

class RHIRenderTargetDescriptor : public RHIDescriptor
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