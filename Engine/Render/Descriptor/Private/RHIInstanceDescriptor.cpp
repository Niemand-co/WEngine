#include "pch.h"
#include "Render/Descriptor/Public/RHIInstanceDescriptor.h"

RHIInstanceDescriptor::RHIInstanceDescriptor()
{
	backend = RHIBackend::None;
	enableDebugLayer = false;
	enableGPUValidator = false;
}

RHIInstanceDescriptor::~RHIInstanceDescriptor()
{
}
