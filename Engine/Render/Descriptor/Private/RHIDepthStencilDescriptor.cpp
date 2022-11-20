#include "pch.h"
#include "Render/Descriptor/Public/RHIDepthStencilDescriptor.h"

RHIDepthStencilDescriptor::RHIDepthStencilDescriptor()
{
	stencilTestEnabled = false;
	depthBoundsTest = false;
	depthTestEnabled = false;
	depthWriteEnabled = false;
	stencilCompareOP = CompareOP::Never;
	stencilFailedOP = StencilFailedOP::Keep;
	passOP = StencilFailedOP::Keep;
	depthFailedOP = StencilFailedOP::Keep;
}