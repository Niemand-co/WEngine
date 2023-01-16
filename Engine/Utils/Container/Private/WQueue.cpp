#include "pch.h"
#include "Utils/Container/Public/WQueue.h"
#include "Utils/Allocator/Public/Allocator.h"

namespace WEngine
{

	void* QueueAllocate(size_t size)
	{
		//return Allocator::Get()->Allocate(size);
		return malloc(size);
	}

	void QueueDeallocate(void* pData)
	{
		//Allocator::Get()->Deallocate(pData);
		return free(pData);
	}

}