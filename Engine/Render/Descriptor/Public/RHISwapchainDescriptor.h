#pragma once

class RHIInstance;

class RHISwapchainDescriptor
{
public:
	
	RHISwapchainDescriptor() = default;

	~RHISwapchainDescriptor() = default;

public:

	unsigned int count = 1;

	Format format = Format::A16R16G16B16_SFloat;

	ColorSpace colorSpace = ColorSpace::SRGB_Linear;

	Extent extent = Extent();

	RHIInstance *instance = nullptr;

};