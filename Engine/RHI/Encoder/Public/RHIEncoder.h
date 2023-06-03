#pragma once

class RHIEncoder
{
public:

	virtual ~RHIEncoder() = default;

	void* operator new(size_t size)
	{
		return GetCPUAllocator()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		GetCPUAllocator()->Deallocate(pData);
	}

};