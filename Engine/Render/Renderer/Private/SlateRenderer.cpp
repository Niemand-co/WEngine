#include "pch.h"
#include "Render/Renderer/Public/SlateRenderer.h"
#include "Utils/ImGui/Public/Gui.h"

SlateRenderer::SlateRenderer(CameraComponent* pInCamera, WViewport* Viewport)
	: ScriptableRenderer(pInCamera, Viewport)
{
	
}

SlateRenderer::~SlateRenderer()
{
}

void SlateRenderer::Render()
{
}
