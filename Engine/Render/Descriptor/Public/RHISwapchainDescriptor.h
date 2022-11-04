#pragma once

class RHISurface;

class RHISwapchainDescriptor
{
public:
	
	RHISwapchainDescriptor();

	~RHISwapchainDescriptor() = default;

public:

	unsigned int count;

	Format format;

	ColorSpace colorSpace;

	Extent extent;

	PresentMode presenMode;

	RHISurface *surface;

	int presentFamilyIndex;

};