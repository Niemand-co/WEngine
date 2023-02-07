#include "pch.h"
#include "Render/Public/RenderTarget.h"
#include "Render/Public/RenderTexture.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Descriptor/Public/RHITextureDescriptor.h"

RenderTarget::RenderTarget(uint32 width, uint32 height, Format format)
{
	m_pDescriptor = (RHITextureDescriptor*)WEngine::Allocator::Get()->Allocate(sizeof(RHITextureDescriptor));
	{
		m_pDescriptor->width = width;
		m_pDescriptor->height = height;
		m_pDescriptor->format = format;
	}
	InitRHIResource();
}

void RenderTarget::InitRHIResource()
{
	WEngine::WSharedPtr<RHITexture>& pTexture = m_pTexture;
	WEngine::WTaskGraph::Get()->EnqueTask(new WEngine::WLambdaTask(true, [&pTexture, this]()
		{
			pTexture = RHIContext::GetDevice()->CreateTexture(m_pDescriptor);
		}
	), WEngine::EThreadProperty::RHIThread);
}

void RenderTarget::ReleaseRHIResource()
{
	m_pTexture = nullptr;
}

void RenderTarget::UpdateRHIResource()
{
}
