#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHISurface;

class RHISwapchainDescriptor : public RHIDescriptor
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