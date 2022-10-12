#pragma once

class RHITextureDescriptor
{
public:

	RHITextureDescriptor();

	virtual ~RHITextureDescriptor();

public:

	Format format;

	unsigned int width;

	unsigned int height;

	unsigned int mipCount;

	unsigned int usage;

	AttachmentLayout layout;

};