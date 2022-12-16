#pragma once
#include "Render/Public/RenderResource.h"

class RHITexture;

class Texture : public RenderResource
{
public:

	Texture();

	virtual ~Texture();

private:

	WEngine::WString m_name;

	unsigned int m_width;

	unsigned int m_height;

	unsigned int m_depth;

	RHITexture *m_pRHITexture;

};

class TextureWithSRV : public Texture
{
public:

	TextureWithSRV();

	virtual ~TextureWithSRV();

};

class TextureWithRTV : public Texture
{
public:

	TextureWithRTV();

	virtual ~TextureWithRTV();

};