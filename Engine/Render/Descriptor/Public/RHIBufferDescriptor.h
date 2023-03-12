#pragma once

class RHIBufferDescriptor
{

public:

	RHIBufferDescriptor() = default;

	~RHIBufferDescriptor() = default;

public:

	uint32 Stride = 0;

	uint32 Count = 0;

	EBufferUsageFlags Usage = EBufferUsageFlags::BF_None;

};