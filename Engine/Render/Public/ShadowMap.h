#pragma once
#include "Render/Public/Texture.h"

class ShadowMap2D : public WTexture2D
{
public:

	ShadowMap2D();

	virtual ~ShadowMap2D();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

};