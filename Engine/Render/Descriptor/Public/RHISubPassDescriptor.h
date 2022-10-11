#pragma once

class RHISubPassDescriptor
{
public:

	~RHISubPassDescriptor();

public:

	unsigned int attachmentIndex;

	AttachmentLayout attachmentLayout;

	int dependedPass;

	unsigned int dependedStage;

	unsigned int dependedAccess;

	unsigned int waitingStage;

	unsigned int waitingAccess;

};