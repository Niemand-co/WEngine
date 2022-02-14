#include "Application.h"
#include "Utils/Shader.h"
#include "Utils/OBJLoader.h"
#include "Camera/Camera.h"
#include "Render/Framebuffer.h"

Application* Application::m_instance = nullptr;

Application::Application()
{
	if(m_instance != nullptr)return;
	m_instance = this;
}

Application::~Application()
{

}

Application* Application::CreateApplication()
{
	if(m_instance == nullptr)
		return new Application();
	else
	{
		std::cout<<"Over Created Application"<<std::endl;
		exit(0);
	}
}

void Application::Init()
{
	Mesh mesh = OBJLoader::Load("assets/sphere.obj");
	std::vector<Vertex> vertices = {
		Vertex(Vec3(-1.0f, -1.0f, 1.0f), Vec3(), Vec3(1.0f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, -1.0f, 1.0f), Vec3(), Vec3(0.0f, 1.0f, 0.0f)),
		Vertex(Vec3(1.0f, -1.0f, -1.0f), Vec3(), Vec3(0.0f, 0.0f, 1.0f)),
		Vertex(Vec3(-1.0f, -1.0f, -1.0f), Vec3(), Vec3(1.0f, 0.0f, 0.0f)),

		Vertex(Vec3(-1.0f, 1.0f, 1.0f), Vec3(), Vec3(0.0f, 1.0f, 0.0f)),
		Vertex(Vec3(1.0f, 1.0f, 1.0f), Vec3(), Vec3(0.0f, 0.0f, 1.0f)),
		Vertex(Vec3(1.0f, 1.0f, -1.0f), Vec3(), Vec3(1.0f, 0.0f, 0.0f)),
		Vertex(Vec3(-1.0f, 1.0f, -1.0f), Vec3(), Vec3(0.0f, 1.0f, 0.0f))
	};
	std::vector<uint32_t> indices = {
		3, 2, 1, 1, 0, 3,
		0, 1, 5, 5, 4, 0,
		1, 2, 6, 6, 5, 1,
		2, 3, 7, 7, 6, 2,
		3, 0, 4, 4, 7, 3,
		4, 5, 6, 6, 7, 4
	};


	const char windowName[] = "SoftRenderer";
	m_window = new Window(windowName, 640, 480);

	Framebuffer::SetClearColor(Vec4(0.2f, 0.2f, 0.2f, 1.0f));
	Framebuffer FBO(640, 480);
	Framebuffer::BindFramebuffer(&FBO);
	FBO.SetBufferType(COLOR_BUFFER, 1);
	FBO.SetDepthTest(true);
	FBO.ClearBuffer(COLOR_BUFFER | DEPTH_BUFFER);

	m_world = World::CreateWorld();
	Entity* sphere = World::CreateEntity(m_world);
	Camera* camera = World::CreateEntity<Camera>(m_world);
	camera->Move(Camera::direction::BACKWARD, 3.0f);
	camera->Move(Camera::direction::RIGHT, 1.0f);
	camera->Move(Camera::direction::UP, 1.0f);
	Mesh Cube;
	Cube.AddVertices(&vertices[0], vertices.size());
	Cube.AddIndices(&indices[0], indices.size());
	sphere->AddComponent<Mesh>(&Cube);
	Material material;
	sphere->AddComponent<Material>(&material);

	Matrix4x4f proj = Matrix4x4f::GetIdentityMatrix();
	PerspectiveProjection(proj, 45.0f, 640.0f / 480.0f, 1.0f, 100.0f);
	Matrix4x4f model = Matrix4x4f::GetIdentityMatrix();
	Scale(model, Vec3(0.5f, 0.5f, 0.5f));
	Shader shader(model, camera->GetViewMatrix(), proj);
	Renderer renderer(Renderer::Primitive::TRIANGLE);
	renderer.SetShader(&shader);
	m_world->SetRenderer(sphere, &renderer);

	m_world->Render();
	FBO.RenderToScreen(m_window);
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