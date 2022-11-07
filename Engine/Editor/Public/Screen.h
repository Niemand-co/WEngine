#pragma once

namespace WEngine
{

	class Screen
	{
	public:
		
		Screen();

		~Screen();

		unsigned int GetWidth();

		unsigned int GetHeight();

	private:

		unsigned int m_width;

		unsigned int m_height;

	};

}