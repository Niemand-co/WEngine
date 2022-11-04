#include "pch.h"
#include "Render/Descriptor/Public/RHIBarrierDescriptor.h"

RHIBarrierDescriptor::RHIBarrierDescriptor()
	: bufferCount(0), pBufferBarriers(nullptr), textureCount(0), pTextureBarriers(0), srcStage(0), dstStage(0)
{
}