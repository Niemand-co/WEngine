#include "pch.h"
#include "Platform/Windows/Public/WThreadWin.h"
#include <windows.h>

namespace WEngine
{

	WThreadWin::WThreadWin()
	{
	}

	WThreadWin::~WThreadWin()
	{
	}

	size_t WThreadWin::GetPid()
	{
		return 0;
	}

}