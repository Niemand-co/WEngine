#include "pch.h"
#include "Event/Public/Input.h"
#include "Event/Public/WindowInput.h"

namespace WEngine
{

	Input* Input::g_pInstance = nullptr;

	void Input::Init()
	{
		g_pInstance = new WindowInput();
	}

	void Input::QueryPlayerInput()
	{
		
	}

	bool Input::IsKeyPressed(int keycode)
	{
		return g_pInstance->IsKeyPressedImpl(keycode);
	}

	glm::vec2 Input::GetMousePosition()
	{
		return g_pInstance->GetMousePositionImpl();
	}

	float Input::GetMouseOffsetX()
	{
		return g_pInstance->GetMouseOffsetXImpl();
	}

	float Input::GetMouseOffsetY()
	{
		return g_pInstance->GetMouseOffsetYImpl();
	}

	float Input::GetMouseOffsetXImpl()
	{
		float currentX = g_pInstance->GetMousePositionImpl().x;
		float offsetX = currentX - m_lastFrameMousePositionX;
		m_lastFrameMousePositionX = currentX;
		return offsetX;
	}

	float Input::GetMouseOffsetYImpl()
	{
		float currentY = g_pInstance->GetMousePositionImpl().y;
		float offsetY = currentY - m_lastFrameMousePositionY;
		m_lastFrameMousePositionY = currentY;
		return offsetY;
	}

}