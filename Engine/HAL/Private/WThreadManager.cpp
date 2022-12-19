#include "pch.h"
#include "HAL/Public/WThreadManager.h"

namespace WEngine
{

    WThreadManager* WThreadManager::g_instance = nullptr;

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