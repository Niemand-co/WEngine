#pragma once

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

	private:

		static unsigned int m_width;

		static unsigned int m_height;

	};

}