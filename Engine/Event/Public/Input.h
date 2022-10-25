#pragma once

namespace WEngine
{

	class Input
	{
	public:

		static void Init();

		virtual ~Input() = default;

		static bool IsKeyPressed(int keycode);

	protected:

		virtual bool IsKeyPressedImpl(int keycoder) = 0;

	private:

		static Input *g_pInstance;

	};

}