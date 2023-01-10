#pragma once

namespace WEngine
{

	

	class WTaskGraph
	{
	public:

		enum class EThreadName : unsigned int
		{
			GameThread   = 0,
			RenderThread = 1,
			RHIThread    = 2,
		};

		WTaskGraph();

		~WTaskGraph();



	private:

		

	};

}