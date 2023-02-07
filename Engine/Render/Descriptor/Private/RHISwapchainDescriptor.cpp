#include "pch.h"
#include "Render/Descriptor/Public/RHISwapchainDescriptor.h"
#include "RHI/Public/RHISurface.h"

RHISwapchainDescriptor::RHISwapchainDescriptor()
{
	count = 3;
	format = Format::A8R8G8B8_UNorm;
	colorSpace = ColorSpace::SRGB_Linear;
	extent = { 1920u, 1080u };
	presenMode = PresentMode::Immediate;

}