#pragma once

class RHITextureViewDescriptor
{
public:

	RHITextureViewDescriptor();

	virtual ~RHITextureViewDescriptor() = default;

public:

	Format format = Format::A16R16G16B16_SFloat;

	uint32 mipCount = 1;

	uint32 baseMipLevel = 0;

	uint32 arrayLayerCount = 1;

	uint32 baseArrayLayer = 0;

	uint32 planeIndex = 0;

	uint32 planeCount = 0;

	Dimension dimension = Dimension::Texture2D;

};