#include "pch.h"
#include "HAL/Public/WEvent.h"
#include "HAL/Public/PlatformProcess.h"
#include "HAL/Public/Platform.h"

namespace WEngine
{

	WEvent* WEvent::Create()
	{
		return PlatformProcess::CreateWEvent();
	}

}