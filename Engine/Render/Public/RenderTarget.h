#pragma once
#include "Render/Public/RenderResource.h"

class RHIRenderTarget;
class RenderTexture;

class RenderTarget : public RenderResource
{
public:

	RenderTarget(uint32 width, uint32 height, Format format);

	~RenderTarget() = default;

private:

	WEngine::WSharedPtr<RHITexture> m_pTexture;

};