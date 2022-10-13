#pragma once

class RHIBuffer;
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

	size_t *pSize;

	size_t *pOffsets;

	BindingResource *pBindingResources;

	RHIGroup *pGroup;

};