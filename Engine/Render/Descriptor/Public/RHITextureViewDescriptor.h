#pragma once

class RHITextureViewDescriptor
{
public:

	RHITextureViewDescriptor();

	virtual ~RHITextureViewDescriptor();

public:

	Format format;

	unsigned int imageAspect;

	unsigned int mipCount;

	unsigned int baseMipLevel;

	unsigned int arrayLayerCount;

	unsigned int baseArrayLayer;

	Dimension dimension;

};