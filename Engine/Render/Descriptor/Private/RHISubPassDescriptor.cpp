#include "pch.h"
#include "Render/Descriptor/Public/RHISubPassDescriptor.h"

RHISubPassDescriptor::RHISubPassDescriptor()
	: colorAttachmentCount(0), pColorAttachments(nullptr), pDepthStencilAttachment(nullptr),
	  inputAttachmentCount(0), pInputAttachments(nullptr), dependedPass(-1), dependedStage(0),
	  dependedAccess(0), waitingStage(0), waitingAccess(0)
{
}

RHISubPassDescriptor::~RHISubPassDescriptor()
{
}
