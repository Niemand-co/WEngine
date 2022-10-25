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

	bool Input::IsKeyPressed(int keycode)
	{
		return g_pInstance->IsKeyPressedImpl(keycode);
	}

}