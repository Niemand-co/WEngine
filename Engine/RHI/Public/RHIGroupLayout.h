#pragma once

struct BindingResource;

class RHIGroupLayout
{
public:

	~RHIGroupLayout() = default;

public:

	unsigned int bindingCount;

	BindingResource *pBindingResources;

};