#pragma once

namespace WEngine
{

	class WTaskGraph
	{
	public:

		enum class EThreadName : unsigned int
		{
			GameThread     = 0u,
			RenderThread   = 1u,
			RHIThread      = 1u << 1,

			HighPriority   = 1u << 2,
			NormalPriority = 1u << 3,
			LowPriority    = 1u << 4,
		};

		WTaskGraph();

		~WTaskGraph();

		template<typename LAMBDA>
		void CreateTask(LAMBDA lambda);

		void AttachToThread(EThreadName name);

	public:

		static WTaskGraph* Get() { return g_instance; }

	private:

		static WTaskGraph *g_instance;

	};

}