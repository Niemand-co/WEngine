#pragma once

class RenderResource
{
public:

	void* operator new(size_t size)
	{
		WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

};