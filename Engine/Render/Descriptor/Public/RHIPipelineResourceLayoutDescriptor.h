#pragma once

class RHIGroupLayout;

class RHIPipelineResourceLayoutDescriptor
{
public:

	RHIPipelineResourceLayoutDescriptor();

	virtual ~RHIPipelineResourceLayoutDescriptor();

public:

	unsigned int groupLayoutCount;

	RHIGroupLayout* pGroupLayout;

};