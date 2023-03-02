#pragma once
#include "Render/Descriptor/Public/RHIAttachmentDescriptor.h"
#include "Render/Descriptor/Public/RHISubPassDescriptor.h"


#define MaxSubPasses                 1

class RHIAttachmentDescriptor;
class RHISubPassDescriptor;
class RHISubPassDependencyDescriptor;

class RHIRenderPassDescriptor
{
public:

	RHIRenderPassDescriptor() = default;

	~RHIRenderPassDescriptor() = default;

	int32 GetRenderPassHash()
	{
		return WEngine::MemCrc32(this, sizeof(RHIRenderPassDescriptor));
	}

public:

	unsigned int ColorAttachmentCount = 0;

	RHIAttachmentDescriptor ColorAttachmentDescriptors[MaxSimultaneousRenderTargets];

	uint8 bHasDepthStencilAttachment : 1 = false;

	RHIAttachmentDescriptor DepthStencilAttachmentDescriptor;

	unsigned int SubpassCount = 0;

	RHISubPassDescriptor SubPassDescriptors[MaxSubPasses];

	unsigned int DependencyCount = 0;

	RHISubPassDependencyDescriptor DependencyDescriptors[MaxSubPasses + 1];

};