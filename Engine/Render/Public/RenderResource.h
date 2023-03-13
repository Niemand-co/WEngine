#pragma once
#include "Utils/Container/Public/WSharedPtr.h"
#include "HAL/Public/TaskGraph.h"
#include "HAL/Public/WEvent.h"

class RenderResource
{
public:
	
	virtual void InitRHIResource() {}

	virtual void ReleaseRHIResource() {}

	virtual void UpdateRHIResource() {}

	bool WaitForRHIResourceReady() { ReasourceReadyEvent->Wait(); }

	void MarkDirty() { bMarkedDirty = true; ReasourceReadyEvent->Reset(); }

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

protected:

	uint8 bMarkedDirty : 1;

	WEngine::WSharedPtr<WEngine::WGraphEvent> ReasourceReadyEvent;

};

/**
 * Start initializing resource.
 * 
 * @param resource The resource wating for initialization
 * @return void
 */
void BeginInitResource(RenderResource* resource);

/**
 * Start releasing resource.
 *
 * @param resource The resource wating for releasing
 * @return void
 */
void BeginReleaseResource(RenderResource* resource);

/**
 * To enque resource waiting for initialization.
 * 
 * @param resource The resource wating for initialization
 * @return void
 */
void EnqueInitResource(RenderResource *resource);

/**
 * To enque resource waiting for releasing.
 *
 * @param resource The resource waiting for releasing
 * @return void
 */
void EnqueReleasResource(RenderResource *resource);

/**
 * To enque resource waiting for updating.
 *
 * @param resource The resource waiting for updating
 * @return void
 */
void EnqueUpdateResource(RenderResource *resource);