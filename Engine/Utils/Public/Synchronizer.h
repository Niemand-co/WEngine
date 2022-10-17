#pragma once

namespace WEngine
{

	class Synchronizer
	{
		enum class Backend
		{
			None = 0,
			Vulkan,
		};

	public:

		virtual ~Synchronizer() = default;

		virtual void SetEvent(unsigned int index) = 0;

		virtual void WaitEvent(unsigned int index) = 0;

	public:

		static void Init(Backend backend);

		static Synchronizer* Get();

	private:

		static Synchronizer *g_pSynchronizer;

	};

}