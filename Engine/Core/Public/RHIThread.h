#pragma once
#include "HAL/Public/WRunnable.h"

namespace WEngine
{

	class WRHIThread : public WRunnable
	{
	public:

		WRHIThread();

		virtual ~WRHIThread();

		virtual void Init() override;

		virtual unsigned int Run() override;

		virtual void Stop() override;

		virtual void Exit() override;

	private:

		void RHIThreadMain();

	public:

		WEvent *m_pRenderingThreadSyncEvent;

	};

}