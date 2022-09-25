#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHITextureViewDescriptor : public RHIDescriptor
{
public:

	RHITextureViewDescriptor();

	virtual ~RHITextureViewDescriptor();

public:

	Format format;

	unsigned int mipCount;

	unsigned int baseMipLevel;

	unsigned int arrayLayerCount;

	unsigned int baseArrayLayer;

	Dimension dimension;

};