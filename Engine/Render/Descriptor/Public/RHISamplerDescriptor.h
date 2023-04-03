#pragma once

class RHISamplerDescriptor
{
public:

	RHISamplerDescriptor();

	~RHISamplerDescriptor() = default;

public:

	EFilter minFilter;

	EFilter magFilter;

};