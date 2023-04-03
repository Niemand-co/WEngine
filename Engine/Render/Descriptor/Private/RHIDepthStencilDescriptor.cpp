#include "pch.h"
#include "Render/Descriptor/Public/RHIDepthStencilDescriptor.h"

RHIDepthStencilDescriptor::RHIDepthStencilDescriptor()
{
	stencilTestEnabled = false;
	depthBoundsTest = false;
	depthTestEnabled = false;
	depthWriteEnabled = false;
	stencilCompareOP = ECompareOP::Never;
	stencilFailedOP = EStencilFailedOP::Keep;
	passOP = EStencilFailedOP::Keep;
	depthFailedOP = EStencilFailedOP::Keep;
}