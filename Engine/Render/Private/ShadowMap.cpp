#include "pch.h"
#include "Render/Public/ShadowMap.h"

WShadowMap2D::WShadowMap2D(uint32 inWidth, uint32 inHeight, Format format)
	: WTexture2D(inWidth, inHeight, format)
{
}

WShadowMap2D::~WShadowMap2D()
{
}

void WShadowMap2D::InitRHIResource()
{
	Texture = GetRenderCommandList()->CreateTexture2D(Width, Height, format, IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT, IMAGE_ASPECT_DEPTH | IMAGE_ASPECT_STENCIL);
}

void WShadowMap2D::ReleaseRHIResource()
{
	Texture = nullptr;
}

void WShadowMap2D::UpdateRHIResource()
{
}
