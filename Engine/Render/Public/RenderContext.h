#pragma once

class RenderContext
{
public:

	virtual ~RenderContext() = default;

	void* operator new(size_t size)
	{
		return GetCPUAllocator()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		GetCPUAllocator()->Deallocate(pData);
	}

};