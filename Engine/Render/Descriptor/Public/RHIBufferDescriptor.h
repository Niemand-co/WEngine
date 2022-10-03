#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHIBufferDescriptor : public RHIDescriptor
{
public:

	RHIBufferDescriptor();

	virtual ~RHIBufferDescriptor();

public:

	uint64_t size;

	void *pData;

};