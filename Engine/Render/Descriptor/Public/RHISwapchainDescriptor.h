#pragma once

class RHIInstance;

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

	RHIInstance *instance;

	int presentFamilyIndex;

};