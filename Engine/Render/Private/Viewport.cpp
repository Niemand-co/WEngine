#include "pch.h"
#include "Render/Public/Viewport.h"
#include "Event/Public/Input.h"

WEditorViewport::WEditorViewport(uint32 width, uint32 height, Format format)
	: WViewport(width, height, format)
{
}

WEditorViewport::~WEditorViewport()
{
}

void WEditorViewport::ProcessInput()
{
	MinPos = WEngine::Input::GetWindowPos();
	MaxPos = MinPos + WEngine::Input::GetWindowSize();

	glm::vec2 mousePos = WEngine::Input::GetMousePosition();
	if (mousePos.x < MinPos.x && mousePos.y < MinPos.y && mousePos.x > MaxPos.x && mousePos.y > MaxPos.y)
	{
		m_bHovered = false;
		return;
	}
	m_bHovered = true;

	if (WEngine::Input::IsMouseClicked(WE_MOUSE_BUTTON_1))
	{
		m_bActive = true;
		m_bClicked = true;
	}
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