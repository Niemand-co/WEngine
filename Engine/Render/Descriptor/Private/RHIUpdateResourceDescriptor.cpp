#include "pch.h"
#include "Render/Descriptor/Public/RHIUpdateResourceDescriptor.h"

RHIUpdateResourceDescriptor::RHIUpdateResourceDescriptor()
	: pBuffer(nullptr), pTextureView(nullptr), pSampler(nullptr)
{
}

RHIUpdateResourceDescriptor::~RHIUpdateResourceDescriptor()
{
}
