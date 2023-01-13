#include "pch.h"
#include "Platform/Windows/Public/WindowsEvent.h"

namespace WEngine
{

	WEventWin::WEventWin()
	{
		Create();
	}

	WEventWin::~WEventWin()
	{
	}

	bool WEventWin::Create()
	{
		m_event = CreateEvent(NULL, true, false, "Test");
		return m_event != NULL;
	}

	void WEventWin::Wait()
	{
		WaitForSingleObject(m_event, INFINITE);
	}

}