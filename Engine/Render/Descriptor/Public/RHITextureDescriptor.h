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

	ETextureCreateFlags Flag = ETextureCreateFlags::TextureCreate_None;

	EAccess initialState;

};