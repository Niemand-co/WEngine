#pragma once

namespace WEngine
{

	class Input
	{
	public:

		static void Init();

		virtual ~Input() = default;

		static void QueryPlayerInput();

		static bool IsKeyPressed(int keycode);

		static glm::vec2 GetMousePosition();

		static float GetMouseOffsetX();

		static float GetMouseOffsetY();

	protected:

		virtual bool IsKeyPressedImpl(int keycoder) = 0;

		virtual glm::vec2 GetMousePositionImpl() = 0;

		virtual float GetMouseOffsetXImpl();

		virtual float GetMouseOffsetYImpl();

	private:

		static Input *g_pInstance;

	private:

		float m_lastFrameMousePositionX;

		float m_lastFrameMousePositionY;

	};

}