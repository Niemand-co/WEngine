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
		while (true)
		{
			WaitForSingleObject(mutex, INFINITE);
			RE_LOG("New Thread");
			ReleaseMutex(mutex);
		}
		return 1;
	}

	virtual void Stop() override
	{

	}

	virtual void Exit() override
	{

	}

};

WEngine::WString GetTime()
{
	time_t timer;
	time(&timer);
	tm date;
	gmtime_s(&date, &timer);
	date.tm_hour = (date.tm_hour + 8) % 24;
	char sDate[32];
	strftime(sDate, sizeof(sDate), "[%F %T]", &date);
	WEngine::WString s = sDate;
	return (WEngine::WString&&)s;
}

int main(int argc, char** argv)
{
	//WEngine::Application* g_pApp = WEngine::CreateApplication();
	//g_pApp->Tick();

	mutex = CreateMutex(NULL, false, "test");

	TestRunnable test;
	WEngine::WThread *pThread = WEngine::WThread::Create(&test);

	GetCurrentThreadId();

	while (true)
	{
		WaitForSingleObject(mutex, INFINITE);
		RE_LOG("Main Thread");
		ReleaseMutex(mutex);
	}
	Sleep(1000);

	return 0;
}