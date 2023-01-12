#pragma once
#include "HAL/Public/WRunnable.h"

namespace WEngine
{

	class WRenderingThread : public WRunnable
	{
	public:

		WRenderingThread();

		virtual ~WRenderingThread();

		virtual void Init() override;

		virtual unsigned int Run() override;

		virtual void Stop() override;

		virtual void Exit() override;

	};

}