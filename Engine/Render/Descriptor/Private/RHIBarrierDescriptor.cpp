#include "pch.h"
#include "Render/Descriptor/Public/RHIBarrierDescriptor.h"
#include "RHI/Public/RHITexture.h"

void RHIBarrierBatch::AddBarrier(const RHIBarrierDescriptor& Barrier)
{
	Batches.Push(Barrier);
}