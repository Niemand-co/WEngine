#pragma once

class RHITextureDescriptor
{
public:

	RHITextureDescriptor() = default;

	~RHITextureDescriptor() = default;

public:

	Format format = Format::A16R16G16B16_SFloat;

	uint32 width = 0;

	uint32 height = 0;

	uint32 depth = 0;

	uint32 mipCount = 1;

	uint32 layerCount = 1;

	uint32 usage = IMAGE_USAGE_COLOR_ATTACHMENT;

	uint32 aspect = IMAGE_ASPECT_COLOR;

};