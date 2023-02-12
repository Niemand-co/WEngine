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

	RHIRenderPassDescriptor();

	~RHIRenderPassDescriptor() = default;

	int32 GetRenderPassHash()
	{
		return WEngine::MemCrc32(this, sizeof(RHIRenderPassDescriptor));
	}

public:

	unsigned int AttachmentCount;

	RHIAttachmentDescriptor AttachmentDescriptors[MaxSimultaneousRenderTargets + 3];

	unsigned int SubpassCount;

	RHISubPassDescriptor SubPassDescriptors[MaxSubPasses];

	unsigned int DependencyCount;

	RHISubPassDependencyDescriptor DependencyDescriptors[MaxSubPasses + 1];

};