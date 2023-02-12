#pragma once

#define MaxSimultaneousRenderTargets 8

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

	unsigned int ColorAttachmentCount;

	SubPassAttachment ColorAttachments[MaxSimultaneousRenderTargets];

	uint8 bHasDepthAttachment : 1;

	SubPassAttachment DepthStencilAttachment;

	unsigned int InputAttachmentCount;

	SubPassAttachment InputAttachments[MaxSimultaneousRenderTargets + 1];

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