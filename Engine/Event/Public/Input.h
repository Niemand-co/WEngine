#pragma once
#include "Event/Public/InputMacro.h"

namespace WEngine
{

	class Input
	{
	public:

		static void Init();

		virtual ~Input() = default;

		static void Poll();

		static bool IsKeyPressed(int32 keycode);

		static bool IsMouseClicked(int32 mouseButton);

		static glm::vec2 GetMousePosition();

		static float GetMouseOffsetX();

		static float GetMouseOffsetY();

		static glm::vec2 GetWindowPos();

		static glm::vec2 GetWindowSize();

	protected:

		virtual bool IsKeyPressedImpl(int32 keycode) = 0;

		virtual bool IsMouseClickedImpl(int32 mouseButton) = 0;

		virtual glm::vec2 GetMousePositionImpl() = 0;

		virtual float GetMouseOffsetXImpl();

		virtual float GetMouseOffsetYImpl();

		virtual glm::vec2 GetWindowPosImpl() = 0;

		virtual glm::vec2 GetWindowSizeImpl() = 0;

		virtual void RegisterEvent(class Event *event);

	private:

		static Input *g_pInstance;

	private:

		float m_lastFrameMousePositionX;

		float m_lastFrameMousePositionY;

	};

}