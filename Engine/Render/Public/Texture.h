#pragma once
#include "Render/Public/RenderResource.h"

class RHITexture;

class WTexture : public RenderResource
{
public:

	WTexture();

	virtual ~WTexture();

private:

	WEngine::WString m_name;

	unsigned int m_width;

	unsigned int m_height;

	unsigned int m_depth;

	RHITexture *m_pRHITexture;

};

class WTexture2D : public WTexture
{
public:

	WTexture2D();

	virtual ~WTexture2D();

protected:

	RHITexture2D

};

class WTextureWithSRV : public WTexture2D
{
public:

	WTextureWithSRV();

	virtual ~WTextureWithSRV();

};

class WTextureWithRTV : public WTexture2D
{
public:

	WTextureWithRTV();

	virtual ~WTextureWithRTV();

};