#pragma once
#include "Render/Public/Texture.h"

class RHIRenderTarget;
class RenderTexture;

class WRenderTarget : public RenderResource
{
public:

	WRenderTarget(uint32 inWidth, uint32 inHeight, Format inFormat);

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

	~WRenderTarget() = default;

protected:

	WTexture2DRHIRef Texture;

};