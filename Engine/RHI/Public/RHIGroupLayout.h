#pragma once

struct BindingResource;

class RHIDescriptorSetLayout
{
public:

	~RHIDescriptorSetLayout() = default;

public:

	unsigned int bindingCount;

	BindingResource *pBindingResources;

};