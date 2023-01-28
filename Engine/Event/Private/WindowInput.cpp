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

	bool WindowInput::IsKeyPressedImpl(int32 keycode)
	{
		return Window::cur_window->IsKeyPressed(keycode);
	}

	bool WindowInput::IsMouseClickedImpl(int32 mouseButton)
	{
		return Window::cur_window->IsMouseClicked(mouseButton);
	}

	glm::vec2 WindowInput::GetMousePositionImpl()
	{
		return Window::cur_window->GetMousePosition();
	}
	glm::vec2 WindowInput::GetWindowPosImpl()
	{
		return Window::cur_window->GetWindowPos();
	}
	glm::vec2 WindowInput::GetWindowSizeImpl()
	{
		return Window::cur_window->GetWindowSize();
	}
}