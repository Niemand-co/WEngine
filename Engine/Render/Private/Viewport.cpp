#include "pch.h"
#include "Render/Public/Viewport.h"
#include "Event/Public/Input.h"

void WViewport::BeginRenderFrame()
{
	GetRenderCommandList()->BeginDrawingViewport(ViewportRHI);
}

void WViewport::InitRHIResource()
{
}

void WViewport::ReleaseRHIResource()
{
	delete ViewportRHI;
}

void WViewport::UpdateRHIResource()
{
}

WEditorViewport::WEditorViewport(uint32 width, uint32 height, EFormat format)
	: WViewport(width, height, format)
{
}

WEditorViewport::~WEditorViewport()
{
}

void WEditorViewport::ProcessInput()
{
}

WSceneViewport::WSceneViewport(uint32 width, uint32 height, EFormat format)
	: WViewport(width, height, format)
{
}

WSceneViewport::~WSceneViewport()
{
}

void WSceneViewport::ProcessInput()
{
}