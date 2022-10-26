#include "pch.h"
#include "Event/Public/WindowInput.h"
#include "Utils/Public/Window.h"

namespace WEngine
{

	WindowInput::WindowInput()
	{
	}

	WindowInput::~WindowInput()
	{
	}

	bool WindowInput::IsKeyPressedImpl(int keycode)
	{
		return Window::cur_window->IsKeyPressed(keycode);
	}

	glm::vec2 WindowInput::GetMousePositionImpl()
	{
		return Window::cur_window->GetMousePosition();
	}
}