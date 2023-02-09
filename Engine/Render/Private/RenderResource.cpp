#include "pch.h"
#include "Render/Public/RenderResource.h"

void BeginInitResource(RenderResource* resource)
{
	WEngine::WTaskGraph::Get()->EnqueTask(new WEngine::WLambdaTask(true, [resource]()
		{
			resource->InitRHIResource();
		}
	), WEngine::EThreadProperty::RenderThread);
}

void BeginReleaseResource(RenderResource* resource)
{
	WEngine::WTaskGraph::Get()->EnqueTask(new WEngine::WLambdaTask(true, [resource]()
		{
			resource->ReleaseRHIResource();
		}
	), WEngine::EThreadProperty::RenderThread);
}

void EnqueInitResource(RenderResource* resource)
{
	ResourcesWitingToInit.Push(resource);
}

void EnqueReleasResource(RenderResource* resource)
{
	ResourcesWaitingToRelease.Push(resource);
}

void EnqueUpdateResource(RenderResource* resource)
{
	ResourcesWaitingToUpdate.Push(resource);
}