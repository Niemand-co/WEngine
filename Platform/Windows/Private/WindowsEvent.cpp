#include "pch.h"
#include "Platform/Windows/Public/WindowsEvent.h"
#include "Utils/Allocator/Public/Allocator.h"

namespace WEngine
{

	WEventWin::WEventWin()
	{
		m_event = CreateEvent(NULL, true, false, "Test");
		ResetEvent(m_event);
	}

	WEventWin::~WEventWin()
	{
		CloseHandle(m_event);
	}

	void WEventWin::Wait()
	{
		WaitForSingleObject(m_event, INFINITE);
	}

	void WEventWin::Reset()
	{
		ResetEvent(m_event);
	}

	void WEventWin::Trigger()
	{
		RE_ASSERT(SetEvent(m_event), "Failed to set event signaled.");
	}

	//void* WEventWin::operator new(size_t size)
	//{
	//	return GetCPUAllocator()->Allocate(size);
	//}

	//void WEventWin::operator delete(void* pData)
	//{
	//	GetCPUAllocator()->Deallocate(pData);
	//}

}