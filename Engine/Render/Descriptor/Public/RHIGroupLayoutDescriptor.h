#pragma once

struct BindingResource
{
	unsigned int bindingSlot;
	ResourceType type;
	unsigned int count;
	unsigned int shaderStage;
};

class RHIGroupLayoutDescriptor
{
public:

	RHIGroupLayoutDescriptor();

	~RHIGroupLayoutDescriptor();

public:

	unsigned int bindingCount;

	BindingResource *pBindingResources;

};