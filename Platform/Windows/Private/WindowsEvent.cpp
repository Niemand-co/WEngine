#include "pch.h"
#include "Platform/Windows/Public/WindowsEvent.h"

namespace WEngine
{

	WEventWin::WEventWin()
	{
		m_event = CreateEvent(NULL, true, false, "Test");
		ResetEvent(m_event);
	}

	WEventWin::~WEventWin()
	{
	}

	void WEventWin::Wait()
	{
		WaitForSingleObject(m_event, INFINITE);
		ResetEvent(m_event);
	}

	void WEventWin::Trigger()
	{
		RE_ASSERT(SetEvent(m_event), "Failed to set event signaled.");
	}

}