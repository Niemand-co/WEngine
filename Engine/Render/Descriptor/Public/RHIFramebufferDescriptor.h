#pragma once

class RHIFramebufferDescriptor
{
public:

	RHIFramebufferDescriptor() = default;

	~RHIFramebufferDescriptor() = default;

public:

	FExtent Extent = { WEngine::NumericLimits<uint32>::Max(), WEngine::NumericLimits<uint32>::Max(), WEngine::NumericLimits<uint32>::Max() };

	uint32 AttachmentCount = 0;

	RHITexture *Attachments[MaxSimultaneousRenderTargets + 1] = {nullptr};

};