#include <iostream>
#include "Core/Application.h"

int main()
{
	Application* app = Application::CreateApplication();
	app->Init();
	app->Tick();
	return 0;
}