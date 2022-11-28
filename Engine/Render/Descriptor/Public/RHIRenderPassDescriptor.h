#pragma once

class RHIAttachmentDescriptor;
class RHISubPassDescriptor;
class RHISubPassDependencyDescriptor;

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

	unsigned int dependencyCount;

	RHISubPassDependencyDescriptor *pDependencyDescriptors;

};