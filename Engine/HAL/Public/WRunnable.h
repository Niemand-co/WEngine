#pragma once

namespace WEngine
{

	class WRunnable
	{
	public:

		virtual ~WRunnable() = default;

		virtual void Init() = 0;

		virtual unsigned int Run() = 0;

		virtual void Stop() = 0;

		virtual void Exit() = 0;

	};

}