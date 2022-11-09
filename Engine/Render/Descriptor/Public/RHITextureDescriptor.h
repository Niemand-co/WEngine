#pragma once

class RHITextureDescriptor
{
public:

	RHITextureDescriptor();

	~RHITextureDescriptor() = default;

public:

	Format format;

	unsigned int width;

	unsigned int height;

	unsigned int mipCount;

	unsigned int layerCount;

	unsigned int usage;

	AttachmentLayout layout;

};