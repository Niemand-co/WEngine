#pragma once

class RHIInstance;

class RHISwapchainDescriptor
{
public:
	
	RHISwapchainDescriptor() = default;

	~RHISwapchainDescriptor() = default;

public:

	unsigned int count = 1;

	EFormat format = EFormat::A16R16G16B16_SFloat;

	ColorSpace colorSpace = ColorSpace::SRGB_Linear;

	FExtent Extent;

	RHIInstance *instance = nullptr;

};