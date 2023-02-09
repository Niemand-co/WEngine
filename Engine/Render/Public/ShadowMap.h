#pragma once
#include "Render/Public/Texture.h"

class WShadowMap2D : public WTexture2D
{
public:

	WShadowMap2D(uint32 inWidth, uint32 inHeight, Format format);

	virtual ~WShadowMap2D();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

};