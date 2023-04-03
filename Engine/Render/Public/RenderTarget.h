#pragma once
#include "Render/Public/RenderResource.h"

class WRenderTarget : public RenderResource
{
public:

	WRenderTarget(uint32 inWidth, uint32 inHeight, Format inFormat);

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

	virtual WTexture2DRHIRef GetHandle() const { return Texture; }

	~WRenderTarget() = default;

protected:

	WTexture2DRHIRef Texture;

};