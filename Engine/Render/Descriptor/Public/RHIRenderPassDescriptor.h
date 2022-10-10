#pragma once

class RHIRenderPassDescriptor
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

	AttachmentLayout initialLayout;

	AttachmentLayout finalLayout;

};