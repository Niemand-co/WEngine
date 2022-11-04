#pragma once

class RHIGroupLayout;

class RHIGroupPoolDescriptor
{
public:

	RHIGroupPoolDescriptor();

	~RHIGroupPoolDescriptor() = default;

public:

	RHIGroupLayout *pGroupLayout;

	unsigned int maxSetCount;

};