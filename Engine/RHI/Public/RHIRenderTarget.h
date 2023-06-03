#pragma once

class RHIRenderTarget
{
public:

	virtual ~RHIRenderTarget() = default;

	virtual unsigned int GetWidth();

	virtual unsigned int GetHeight();

	void* operator new(size_t size)
	{
		return GetCPUAllocator()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		GetCPUAllocator()->Deallocate(pData);
	}

protected:

	unsigned int m_width;

	unsigned int m_height;

};