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

	int dependedPass;

	unsigned int dependedStage;

	unsigned int dependedAccess;

	unsigned int waitingStage;

	unsigned int waitingAccess;

};