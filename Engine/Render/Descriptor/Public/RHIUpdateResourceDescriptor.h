#pragma once

class RHIBuffer;
class RHITextureView;
class RHISampler;
class RHIGroup;
class RHIGroupLayout;
struct BindingResource;

class RHIUpdateResourceDescriptor
{
public:

	RHIUpdateResourceDescriptor();

	~RHIUpdateResourceDescriptor();

public:

	unsigned int bindingCount;

	RHIBuffer *pBuffer;

	RHITextureView *pTextureView;

	RHISampler *pSampler;

	size_t *pSize;

	size_t *pOffsets;

	BindingResource *pBindingResources;

	RHIGroup *pGroup;

};