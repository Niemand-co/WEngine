#pragma once

struct BindingResource;

class RHIGroupLayout
{
public:

	virtual ~RHIGroupLayout() = default;

public:

	unsigned int bindingCount;

	BindingResource *pBindingResources;

};