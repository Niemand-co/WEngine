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

	public:

		class WEvent* pMainThreadSyncEvent;

	private:

		void RenderThreadMain();

	private:

		class WRHIThread *m_pRHIRunnable;

		class WThread *m_pRHIThread;

	};

}