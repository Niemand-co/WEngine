#pragma once

class RHIBufferDescriptor
{

public:

	RHIBufferDescriptor() = default;

	~RHIBufferDescriptor() = default;

public:

	uint32 Stride = 0;

	uint32 Count = 0;

	uint8 *Data;

	EBufferUsageFlags Usage = EBufferUsageFlags::BF_None;

};