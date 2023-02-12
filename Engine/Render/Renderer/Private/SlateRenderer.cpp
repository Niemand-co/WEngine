#include "pch.h"
#include "Render/Renderer/Public/SlateRenderer.h"
#include ""

SlateRenderer::SlateRenderer(CameraComponent* pInCamera)
	: ScriptableRenderer(pInCamera)
{
	
}

SlateRenderer::~SlateRenderer()
{
}

void SlateRenderer::Render()
{
	
	Gui::RenderGUI();
}
