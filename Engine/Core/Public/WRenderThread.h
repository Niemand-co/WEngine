#pragma once
#include "Core/Public/WNamedThread.h"

namespace WEngine
{

	class WRenderThread : public WNamedThread
	{
	public:

		WRenderThread();

		virtual ~WRenderThread() = default;

		virtual void Init() override;

		virtual unsigned int Run() override;

		virtual void Stop() override;

		virtual void Exit() override;

	private:

		

	};

}