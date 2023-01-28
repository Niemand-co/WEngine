#pragma once
#include "Event/Public/Input.h"

namespace WEngine
{

	class WindowInput : public Input
	{
	public:

		WindowInput();

		virtual ~WindowInput();

	protected:

		virtual bool IsKeyPressedImpl(int32 keycode) override;

		virtual bool IsMouseClickedImpl(int32 mouseButton) override;

		virtual glm::vec2 GetMousePositionImpl() override;

		virtual glm::vec2 GetWindowPosImpl() override;

		virtual glm::vec2 GetWindowSizeImpl() override;

	};

}