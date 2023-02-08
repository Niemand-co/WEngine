#include "pch.h"
#include "Core/Public/AnyThread.h"

WEngine::WAnyThread::WAnyThread(EThreadProperty inProperty)
	: m_property(inProperty)
{
}

WEngine::WAnyThread::~WAnyThread()
{
}

void WEngine::WAnyThread::Init()
{
}

unsigned int WEngine::WAnyThread::Run()
{
	AnyThreadMain();
	return 0;
}

void WEngine::WAnyThread::Stop()
{
}

void WEngine::WAnyThread::Exit()
{
}

void WEngine::WAnyThread::AnyThreadMain()
{
	WTaskGraph::Get()->AttachToThread(m_property);

	WTaskGraph::Get()->ProcessUntilQuit(m_property);
}
