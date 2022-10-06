#pragma once

class RHIGroupLayout;

class RHIGroupPoolDescriptor
{
public:

	RHIGroupPoolDescriptor();

	~RHIGroupPoolDescriptor();

public:

	RHIGroupLayout *pGroupLayout;

	unsigned int maxSetCount;

};