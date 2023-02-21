#pragma once

class RHITextureViewDescriptor
{
public:

	RHITextureViewDescriptor();

	virtual ~RHITextureViewDescriptor() = default;

public:

	Format format = Format::A16R16G16B16_SFloat;

	unsigned int mipCount = 1;

	unsigned int baseMipLevel = 0;

	unsigned int arrayLayerCount = 1;

	unsigned int baseArrayLayer = 0;

	Dimension dimension = Dimension::Texture2D;

};