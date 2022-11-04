#pragma once

class RHIAttachmentDescriptor;
class RHISubPassDescriptor;

class RHIRenderPassDescriptor
{
public:

	RHIRenderPassDescriptor();

	~RHIRenderPassDescriptor() = default;

public:

	unsigned int attachmentCount;

	RHIAttachmentDescriptor *pAttachmentDescriptors;

	unsigned int subpassCount;

	RHISubPassDescriptor *pSubPassDescriptors;

};