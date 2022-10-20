#pragma once

class RHISamplerDescriptor
{
public:

	RHISamplerDescriptor();

	virtual ~RHISamplerDescriptor();

public:

	Filter minFilter;

	Filter magFilter;

};