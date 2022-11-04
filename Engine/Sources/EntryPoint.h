#include "Utils/Reflection/Public/Register.h"
extern WEngine::Application* WEngine::CreateApplication();

int main(int argc, char** argv)
{
	WEngine::Application* g_pApp = WEngine::CreateApplication();
	g_pApp->Tick();

	return 0;
}