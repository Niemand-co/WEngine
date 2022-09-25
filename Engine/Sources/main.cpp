#include "pch.h"
#include "Framework/Public/Application.h"

int main(int argc, char** argv)
{
	Application* g_pApp = Application::CreateApplication();
	g_pApp->Init();
	g_pApp->Tick();
	return 0;
}