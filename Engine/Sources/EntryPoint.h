extern WEngine::Application* WEngine::CreateApplication();

int main(int argc, char** argv)
{
	//WEngine::Application* g_pApp = WEngine::CreateApplication();
	//g_pApp->Tick();

	WEngine::WDeque<int> q;
	q.AtomicPushBack(1);
	RE_LOG(q.Front());
	q.AtomicPushBack(2);
	RE_LOG(q.Front());
	q.AtomicPopFront();
	RE_LOG(q.Front());

	return 0;
}