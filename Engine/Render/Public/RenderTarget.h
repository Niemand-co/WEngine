#pragma once
#include "Render/Public/RenderResource.h"

class WRenderTarget : public RenderResource
{
public:

	WRenderTarget(uint32 inWidth, uint32 inHeight, EFormat inFormat);

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

	virtual WTextureRHIRef GetHandle() const { return Texture; }

	~WRenderTarget() = default;

protected:

	WTextureRHIRef Texture;

};