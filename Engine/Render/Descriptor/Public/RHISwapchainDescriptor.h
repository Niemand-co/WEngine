#pragma once

class RHISurface;

class RHISwapchainDescriptor
{
public:
	
	RHISwapchainDescriptor();

	virtual ~RHISwapchainDescriptor();

public:

	unsigned int count;

	Format format;

	ColorSpace colorSpace;

	Extent extent;

	PresentMode presenMode;

	RHISurface *surface;

	int presentFamilyIndex;

};