#include "pch.h"
#include "Render/Public/Texture.h"

WTextureWithSRV::WTextureWithSRV(uint32 inWidth, uint32 inHeight, Format inFormat)
	: WTexture2D(inWidth, inHeight, inFormat)
{
}

void WTextureWithSRV::InitRHIResource()
{
}

void WTextureWithSRV::ReleaseRHIResource()
{
	Texture = nullptr;
}

void WTextureWithSRV::UpdateRHIResource()
{
}
