#include "pch.h"
#include "Render/Public/Texture.h"

WTextureWithSRV::WTextureWithSRV(uint32 inWidth, uint32 inHeight, Format inFormat)
	: WTexture2D(inWidth, inHeight, inFormat)
{
}

void WTextureWithSRV::InitRHIResource()
{
	Texture = GetRenderCommandList()->CreateTexture2D(Width, Height, format, IMAGE_USAGE_SAMPLED, IMAGE_ASPECT_COLOR);
}

void WTextureWithSRV::ReleaseRHIResource()
{
	Texture = nullptr;
}

void WTextureWithSRV::UpdateRHIResource()
{
}
