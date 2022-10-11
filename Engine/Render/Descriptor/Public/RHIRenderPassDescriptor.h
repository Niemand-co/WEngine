#pragma once

class RHIAttachmentDescriptor;
class RHISubPassDescriptor;

class RHIRenderPassDescriptor
{
public:

	RHIRenderPassDescriptor();

	virtual ~RHIRenderPassDescriptor();

public:

	unsigned int attachmentCount;

	RHIAttachmentDescriptor *pAttachmentDescriptors;

	unsigned int subpassCount;

	RHISubPassDescriptor *pSubPassDescriptors;

};