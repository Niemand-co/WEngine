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
}

void WShadowMap2D::ReleaseRHIResource()
{
	Texture = nullptr;
}

void WShadowMap2D::UpdateRHIResource()
{
}
