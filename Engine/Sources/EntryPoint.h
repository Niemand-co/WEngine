#include "HAL/Public/WRunnable.h"
extern WEngine::Application* WEngine::CreateApplication();

HANDLE mutex;

class TestRunnable : public WEngine::WRunnable
{
public:

	TestRunnable() = default;

	virtual ~TestRunnable() = default;

	virtual void Init() override
	{

	}

	virtual unsigned int Run() override
	{
		WaitForSingleObject(mutex, INFINITE);
		RE_LOG("New Thread");
		ReleaseMutex(mutex);
		return 1;
	}

	virtual void Stop() override
	{

	}

	virtual void Exit() override
	{

	}

};

int main(int argc, char** argv)
{
	//WEngine::Application* g_pApp = WEngine::CreateApplication();
	//g_pApp->Tick();

	mutex = CreateMutex(NULL, false, "test");

	TestRunnable test;
	WEngine::WThread *pThread = WEngine::WThreadManager::CreateThread(&test);

	WaitForSingleObject(mutex, INFINITE);
	RE_LOG("Main Thread");
	ReleaseMutex(mutex);
	Sleep(1000);

	return 0;
}