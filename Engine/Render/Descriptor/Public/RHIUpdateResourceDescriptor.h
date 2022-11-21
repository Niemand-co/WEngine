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

struct RHIBindingDescriptor
{
	unsigned int bufferResourceCount;

	BufferResourceInfo *pBufferResourceInfo;
};

class RHIUpdateResourceDescriptor
{
public:

	RHIUpdateResourceDescriptor();

	~RHIUpdateResourceDescriptor() = default;

public:

	unsigned int bindingCount;

	BindingResource *pBindingResources;

	RHIBindingDescriptor *pBindingDescriptors;

	unsigned int textureResourceCount;

	TextureResourceInfo *pTextureInfo;

	RHIGroup *pGroup;

};