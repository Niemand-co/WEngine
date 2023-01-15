#pragma once
#include "Render/Public/RenderResource.h"

class RHIRenderTarget;
class RenderTexture;

class RenderTarget : public RenderResource
{
public:

	RenderTarget();

	~RenderTarget() = default;

private:

	//WEngine::WSharedPtr<RHITexture> m_pTexture;

};