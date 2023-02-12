#include "pch.h"
#include "Render/Public/RenderTarget.h"
#include "Render/Public/RenderTexture.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Descriptor/Public/RHITextureDescriptor.h"

WRenderTarget::WRenderTarget(uint32 inWidth, uint32 inHeight, Format inFormat)
	: WTexture2D(inWidth, inHeight, inFormat)
{
}

void WRenderTarget::InitRHIResource()
{
	Texture = GetRenderCommandList()->CreateTexture2D(Width, Height, format, IMAGE_USAGE_COLOR_ATTACHMENT | IMAGE_USAGE_SAMPLED, IMAGE_ASPECT_COLOR);
	ReasourceReadyEvent->Trigger();
}

void WRenderTarget::ReleaseRHIResource()
{
	Texture = nullptr;
}

void WRenderTarget::UpdateRHIResource()
{
}
