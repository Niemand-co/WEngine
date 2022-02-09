#include "Application.h"
#include "Utils/Shader.h"

Application* Application::m_instance = nullptr;

Application::Application()
{
	if(m_instance != nullptr)return;
	m_instance = this;
}

Application::~Application()
{

}

void Application::Init()
{
	const char windowName[] = "SoftRenderer";
	m_window = new Window(windowName, 1280, 720);

	Vertex V1(Vec3(-0.5, -0.5, 0), Vec4(0, 0, 0, 0), Vec4(1, 0, 0, 0));
	Vertex V2(Vec3(0.5, -0.5, 0), Vec4(0, 0, 0, 0), Vec4(0, 1, 0, 0));
	Vertex V3(Vec3(0, 0.5, 0), Vec4(0, 0, 0, 0), Vec4(0, 0, 1, 0));

	Shader shader;

	V2F o1 = shader.VertexShader(V1);
	V2F o2 = shader.VertexShader(V2);
	V2F o3 = shader.VertexShader(V3);

	m_rasterizer = new Rasterizer(m_window);
	m_rasterizer->RasterizeTriangle(o1, o2, o3);
}

void Application::Tick()
{
	MSG msg;
	while (true)
	{
		if (GetMessage(&msg, m_window->GetWindowHandle(), 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}