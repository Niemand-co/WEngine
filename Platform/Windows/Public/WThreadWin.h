#pragma once
#include "HAL/Public/WThread.h"

namespace WEngine
{

	class WThreadWin : public WThread
	{
	public:

		WThreadWin();

		virtual ~WThreadWin();

		size_t GetPid();

	};

}