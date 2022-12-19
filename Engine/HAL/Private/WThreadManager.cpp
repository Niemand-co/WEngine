#include "pch.h"
#include "HAL/Public/WThreadManager.h"
#include "Platform/Windows/Public/WThreadWin.h"

namespace WEngine
{

    WThreadManager* WThreadManager::g_instance = nullptr;

    WThread* WThreadManager::CreateThread(WRunnable* pRunnable, size_t threadStackSize, WThread::ThreadPriority priority)
    {
        #if defined(WINDOWS) || defined(_WIN) || defined(_WIN32)
            return new WThreadWin(pRunnable, threadStackSize, priority);
        #else
            return nullptr;
        #endif
    }

    void WThreadManager::AddThread(unsigned long threadID, WThread* pThread)
    {
        m_threads.Insert(threadID, pThread);
    }

    WThreadManager* WThreadManager::Get()
    {
        if(g_instance == nullptr)
            g_instance = new WThreadManager();
        return g_instance;
    }

}