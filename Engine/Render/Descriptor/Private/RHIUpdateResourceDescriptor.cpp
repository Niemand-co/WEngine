#include "pch.h"
#include "Render/Descriptor/Public/RHIUpdateResourceDescriptor.h"

RHIUpdateResourceDescriptor::RHIUpdateResourceDescriptor()
	: pBufferInfo(nullptr), pTextureInfo(nullptr), bufferResourceCount(0), textureResourceCount(0)
{
	dynamicBufferCount = 0;
}