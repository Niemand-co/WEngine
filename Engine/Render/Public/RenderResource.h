#pragma once

class RenderResource
{
public:
	
	virtual void InitRHIResource() = 0;

	virtual void ReleaseRHIResource() = 0;

	virtual void UpdateRHIResource() = 0;

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

};

/**
* To enque resource waiting for initialization.
* 
* @param resource The resource wating for initialization
* @return void
*/
void BeginInitResource(RenderResource* resource)
{
	WEngine::WTaskGraph::Get()->EnqueTask(new WEngine::WLambdaTask(true, [&resource]()
	{
		resource->InitRHIResource();
	}
	), WEngine::EThreadProperty::RenderThread);
}