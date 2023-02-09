#pragma once
#include "Render/Public/Texture.h"

class RHIRenderTarget;
class RenderTexture;

class WRenderTarget : public WTexture2D
{
public:

	WRenderTarget(uint32 inWidth, uint32 inHeight, Format inFormat);

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

	~WRenderTarget() = default;

private:

	WTexture2DRHIRef Texture;

};