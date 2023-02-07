#pragma once
#include "Render/Public/RenderResource.h"

class RHIRenderTarget;
class RenderTexture;

class RenderTarget : public RenderResource
{
public:

	RenderTarget(uint32 width, uint32 height, Format format);

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

	~RenderTarget() = default;

private:

	WEngine::WSharedPtr<class RHITexture> m_pTexture;

	class RHITextureDescriptor *m_pDescriptor;

};