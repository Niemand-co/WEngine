#pragma once

class RHISamplerDescriptor
{
public:

	RHISamplerDescriptor();

	~RHISamplerDescriptor() = default;

public:

	Filter minFilter;

	Filter magFilter;

};