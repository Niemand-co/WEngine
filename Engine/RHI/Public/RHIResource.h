#pragma once
#include "Utils/Allocator/Public/Allocator.h"

class RHIResource
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

};