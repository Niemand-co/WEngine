#pragma once

struct SubPassAttachment
{
	unsigned int attachmentIndex;

	AttachmentLayout attachmentLayout;
};

class RHISubPassDescriptor
{
public:

	RHISubPassDescriptor();

	~RHISubPassDescriptor() = default;

public:

	unsigned int colorAttachmentCount;

	SubPassAttachment *pColorAttachments;

	SubPassAttachment *pDepthStencilAttachment;

	unsigned int inputAttachmentCount;

	SubPassAttachment *pInputAttachments;

};

class RHISubPassDependencyDescriptor
{
public:

	~RHISubPassDependencyDescriptor() = default;

public:

	int dependedPass;

	int dependedStage;

	int dependedAccess;

	int waitingPass;

	int waitingStage;

	int waitingAccess;
	
};