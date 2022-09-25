#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHIRenderPassDescriptor : public RHIDescriptor
{
public:

	RHIRenderPassDescriptor();

	virtual ~RHIRenderPassDescriptor();

public:

	unsigned int attachmentCount;

	Format attachmentFormat;

	unsigned int sampleCount;

	AttachmentLoadOP attachmentLoadOP;

	AttachmentStoreOP attachmentStoreOP;

	AttachmentLoadOP stencilLoadOP;

	AttachmentStoreOP stencilStoreOP;

	ImageLayout initialLayout;

	ImageLayout finalLayout;

};