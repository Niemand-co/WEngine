#include "pch.h"
#include "Render/Public/Viewport.h"
#include "Event/Public/Input.h"

WEditorViewport::WEditorViewport()
{
}

WEditorViewport::~WEditorViewport()
{
}

void WEditorViewport::ProcessInput()
{
	m_minPos = WEngine::Input::GetWindowPos();
	m_maxPos = m_minPos + WEngine::Input::GetWindowSize();

	glm::vec2 mousePos = WEngine::Input::GetMousePosition();
	if (mousePos.x < m_minPos.x && mousePos.y < m_minPos.y && mousePos.x > m_maxPos.x && mousePos.y > m_maxPos.y)
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

WSceneViewport::WSceneViewport()
{
}

WSceneViewport::~WSceneViewport()
{
}

void WSceneViewport::ProcessInput()
{
}