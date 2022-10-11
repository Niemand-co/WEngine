#pragma once

class RHIAttachmentDescriptor
{
public:

	~RHIAttachmentDescriptor();

public:

	Format attachmentFormat;

	unsigned int sampleCount;

	AttachmentLoadOP attachmentLoadOP;

	AttachmentStoreOP attachmentStoreOP;

	AttachmentLoadOP stencilLoadOP;

	AttachmentStoreOP stencilStoreOP;

	AttachmentLayout initialLayout;

	AttachmentLayout finalLayout;

};