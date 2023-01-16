#include "HAL/Public/WRunnable.h"
#include "HAL/Public/WThread.h"
extern WEngine::Application* WEngine::CreateApplication();

WEngine::WDeque<int> q;

class test : public WEngine::WRunnable
{
public:

	test() = default;

	virtual ~test() = default;

	void Init() override {}

	unsigned int Run() override { while(true){if(!q.Empty())RE_LOG(q.Front()); q.AtomicPushBack(1);} return 0;}

	void Exit() override {}

	void Stop() override {}

};

int main(int argc, char** argv)
{
	//WEngine::Application* g_pApp = WEngine::CreateApplication();
	//g_pApp->Tick();

	WEngine::Allocator::Init(WEngine::Backend::Vulkan);
	test *t = new test();

	WEngine::WThread *testThread = WEngine::WThread::Create(t, "TEST");

	while (true)
	{
		q.AtomicPushBack(2);
		q.AtomicPushBack(3);
		q.AtomicPopFront();
	}

	return 0;
}