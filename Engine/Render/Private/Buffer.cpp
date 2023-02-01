#include "pch.h"
#include "Render/Public/Buffer.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "RHI/Public/RHIContext.h"

WIndexBuffer::WIndexBuffer()
{
}

WIndexBuffer::~WIndexBuffer()
{
}

void WIndexBuffer::InitRHIResource()
{
	WEngine::WSharedPtr<RHIBuffer>& pBuffer = m_pBuffer;
	WEngine::WTaskGraph::Get()->EnqueTask(new WEngine::WLambdaTask(true, [&pBuffer]()
	{
		RHIBufferDescriptor descriptor = {};
		{
		}
		pBuffer = RHIContext::GetContext()->CreateIndexBuffer(&descriptor);
	}
	), WEngine::EThreadProperty::RHIThread);
}

void WIndexBuffer::ReleaseRHIResource()
{
}

void WIndexBuffer::UpdateRHIResource()
{
}
