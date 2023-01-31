#include "pch.h"
#include "Render/Public/RenderTarget.h"
#include "Render/Public/RenderTexture.h"
#include "RHI/Public/RHIContext.h"

RenderTarget::RenderTarget(uint32 width, uint32 height)
{
	RHITextureDescriptor textureDescriptor = {};
	{
		textureDescriptor.width = width;
		textureDescriptor.height = height;
	}
	WEngine::WSharedPtr<RHITexture>& pTexture = m_pTexture;
	WEngine::WTaskGraph::Get()->EnqueTask(new WEngine::WLambdaTask(true, [&pTexture, &textureDescriptor]()
	{
			pTexture = RHIContext::GetDevice()->CreateTexture(&textureDescriptor);
	}
	), WEngine::EThreadProperty::RHIThread);
}
