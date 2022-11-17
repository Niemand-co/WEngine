#pragma once

class RHIGroupLayout;

class RHIGroupDescriptor
{
public:

	RHIGroupDescriptor();

	~RHIGroupDescriptor() = default;

public:

	RHIGroupLayout *pGroupLayout;

	unsigned int count;

};