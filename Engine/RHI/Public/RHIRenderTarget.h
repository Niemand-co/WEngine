#pragma once

class RHIRenderTarget
{
public:

	virtual ~RHIRenderTarget() = default;

	virtual unsigned int GetWidth();

	virtual unsigned int GetHeight();

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

protected:

	unsigned int m_width;

	unsigned int m_height;

};