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

		virtual bool IsKeyPressedImpl(int keycode) override;

	};

}