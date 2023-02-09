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

void BeginReleasResource(RenderResource* resource)
{
	WEngine::WTaskGraph::Get()->EnqueTask(new WEngine::WLambdaTask(true, [resource]()
		{
			resource->ReleaseRHIResource();
		}
	), WEngine::EThreadProperty::RenderThread);
}