#pragma once

class RHIBuffer;
class RHITextureView;
class RHISampler;
class RHIGroup;
class RHIGroupLayout;
struct BindingResource;

struct BufferResourceInfo
{
	RHIBuffer *pBuffer;
	size_t offset;
	size_t range;
};

struct TextureResourceInfo
{
	RHITextureView *pTextureView;
	RHISampler *pSampler;
	AttachmentLayout layout;
};

class RHIUpdateResourceDescriptor
{
public:

	RHIUpdateResourceDescriptor();

	~RHIUpdateResourceDescriptor() = default;

public:

	unsigned int bindingCount;

	BindingResource *pBindingResources;

	unsigned int dynamicBufferCount;

	unsigned int *bufferResourceCount;

	BufferResourceInfo **pBufferInfo;

	unsigned int textureResourceCount;

	TextureResourceInfo *pTextureInfo;

	RHIGroup *pGroup;

};