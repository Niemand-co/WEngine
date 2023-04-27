#pragma once

class RHIAttachmentDescriptor
{
public:

	~RHIAttachmentDescriptor() = default;

public:

	EFormat attachmentFormat;

	unsigned int sampleCount;

	EAttachmentLoadOP attachmentLoadOP;

	EAttachmentStoreOP attachmentStoreOP;

	EAttachmentLoadOP stencilLoadOP;

	EAttachmentStoreOP stencilStoreOP;

};