#include "pch.h"
#include "Render/Descriptor/Public/RHISubPassDescriptor.h"

RHISubPassDescriptor::RHISubPassDescriptor()
	: colorAttachmentCount(0), pColorAttachments(nullptr), pDepthStencilAttachment(nullptr),
	  inputAttachmentCount(0), pInputAttachments(nullptr)
{
}