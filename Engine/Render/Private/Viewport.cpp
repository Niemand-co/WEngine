#include "pch.h"
#include "Render/Public/Viewport.h"
#include "Event/Public/Input.h"

void WViewport::InitRHIResource()
{
	DepthStencil = GetRenderCommandList()->CreateTexture2D(Width, Height, Format::D32_SFLOAT_S8_UINT, 1, ETextureCreateFlags::TextureCreate_DepthStencil);
}

void WViewport::ReleaseRHIResource()
{
	delete DepthStencil;
	delete ViewportRHI;
}

void WViewport::UpdateRHIResource()
{
}

WEditorViewport::WEditorViewport(uint32 width, uint32 height, Format format)
	: WViewport(width, height, format)
{
}

WEditorViewport::~WEditorViewport()
{
}

void WEditorViewport::ProcessInput()
{
}

WSceneViewport::WSceneViewport(uint32 width, uint32 height, Format format)
	: WViewport(width, height, format)
{
}

WSceneViewport::~WSceneViewport()
{
}

void WSceneViewport::ProcessInput()
{
}