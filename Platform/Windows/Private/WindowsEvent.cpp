#include "pch.h"
#include "Platform/Windows/Public/WindowsEvent.h"

namespace WEngine
{

	WEventWin::WEventWin()
	{
		m_event = CreateEvent(NULL, true, false, "Test");
	}

	WEventWin::~WEventWin()
	{
	}

	void WEventWin::Wait()
	{
		WaitForSingleObject(m_event, INFINITE);
	}

	void WEventWin::Trigger()
	{
		RE_ASSERT(SetEvent(m_event), "Failed to set event signaled.");
	}

}