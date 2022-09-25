#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHITextureDescriptor : public RHIDescriptor
{
public:

	RHITextureDescriptor();

	virtual ~RHITextureDescriptor();

public:

	Format format;

	unsigned int width;

	unsigned int height;

	unsigned int mipCount;

};