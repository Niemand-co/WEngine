#pragma once

class RHIPipelineResourceLayoutDescriptor
{
public:

	RHIPipelineResourceLayoutDescriptor();

	~RHIPipelineResourceLayoutDescriptor() = default;

public:

	unsigned int groupLayoutCount;

	RHIGroupLayout* pGroupLayout;

};