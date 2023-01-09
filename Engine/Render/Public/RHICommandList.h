#pragma once

class RenderTarget;
class RHIGraphicsEncoder;

class RHICommandListBase
{
public:

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

protected:

	virtual ~RHICommandListBase() = default;

};

template<typename T>
class RHICommandList : public RHICommandListBase
{
public:

	virtual ~RHICommandList() = default;

};