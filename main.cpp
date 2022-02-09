#include <iostream>
#include "Core/Application.h"

int main()
{
	Application* app = new Application();
	app->Init();
	app->Tick();
	return 0;
}