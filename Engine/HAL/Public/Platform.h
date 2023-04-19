#pragma once

#if defined(WINDOWS) || defined(_WIN32)
#include "Platform/Windows/Public/WindowsPlatformProcess.h"
#include "Platform/Windows/Public/WindowsCriticalSection.h"
#include "Platform/Windows/Public/WindowsRWLock.h"
#include "Platform/Windows/Public/WindowsEvent.h"
typedef WEngine::WCriticalSectionWin WCriticalSection ;
typedef WEngine::WEventWin WEvent;
typedef WEngine::WRWLockWin WRWLock;
#endif