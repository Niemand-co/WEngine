#pragma once

class Camera;

namespace WEngine
{

	class Screen
	{
	public:
		
		Screen();

		~Screen() = default;

		static void SetWidth(unsigned int width);

		static void SetHeight(unsigned int height);

		static unsigned int GetWidth();

		static unsigned int GetHeight();

		static bool SizeChanged();

		static void ResetState();

		static void SetDisplayCamera(Camera *pCamera);

	public:

		static Camera *g_displayingCamera;

	private:

		static unsigned int g_width;

		static unsigned int g_height;

		static bool g_isSizeChanged;

	};

}