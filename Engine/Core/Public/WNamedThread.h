#pragma once
#include "HAL/Public/WRunnable.h"

namespace WEngine
{

	namespace ENamedThreads
	{
		enum NNameThread : int
		{
			AnyThread = 0,
			RenderThread = 1,
			RHIThread = 2,
		};
	}

	class WNamedThread : public WRunnable
	{
	public:

		WNamedThread() = default;

		virtual ~WNamedThread() = default;

		virtual void Init() override = 0;

		virtual unsigned int Run() override = 0;

		virtual void Stop() override = 0;

		virtual void Exit() override = 0;

	protected:

		

	};

}