#pragma once

#if defined(WINDOWS) || defined(_WIN32)
#include "Platform/Windows/Public/WindowsPlatformProcess.h"
#include "Platform/Windows/Public/WindowsCriticalSection.h"
typedef WEngine::WCriticalSectionWin WCriticalSection ;
#endif