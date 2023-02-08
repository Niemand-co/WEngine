#pragma once
#include "HAL/Public/WRunnable.h"

namespace WEngine
{

	class WAnyThread : public WRunnable
	{
	public:

		WAnyThread(EThreadProperty inProperty);

		virtual ~WAnyThread();

		virtual void Init() override;

		virtual unsigned int Run() override;

		virtual void Stop() override;

		virtual void Exit() override;

	private:

		void AnyThreadMain();

	private:

		EThreadProperty m_property;

	};

}