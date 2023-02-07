#pragma once
#include "Utils/Container/Public/WSharedPtr.h"
#include "HAL/Public/TaskGraph.h"
#include "HAL/Public/WEvent.h"

class RenderResource
{
public:
	
	virtual void InitRHIResource() = 0;

	virtual void ReleaseRHIResource() = 0;

	virtual void UpdateRHIResource() = 0;

	bool WaitForRHIResourceReady() { ReasourceReadyEvent->Wait(); }

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

private:

	WEngine::WSharedPtr<WEngine::WGraphEvent> ReasourceReadyEvent;

};

/**
* To enque resource waiting for initialization.
* 
* @param resource The resource wating for initialization
* @return void
*/
void BeginInitResource(RenderResource* resource);