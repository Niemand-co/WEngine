#include "Application.h"
#include "Utils/Shader.h"
#include "Camera/Camera.h"
#include "Scene/Light.h"
#include "Platform/Windows/WindowsShader.h"
#include "Render/RenderCommand.h"
#include <windows.h>

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
	Mesh* Sphere = MeshLibrary::Allocate("assets/sphere.obj");
	std::vector<Vertex> vertices = {
		Vertex(Vec3(-1.0f, -1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, -1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, -1.0f, -1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(-1.0f, -1.0f, -1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),

		Vertex(Vec3(-1.0f, -1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, -1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(-1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.5f, 0.0f, 0.0f)),

		Vertex(Vec3(1.0f, -1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, -1.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, 1.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),

		Vertex(Vec3(1.0f, -1.0f, -1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(-1.0f, -1.0f, -1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(-1.0f, 1.0f, -1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, 1.0f, -1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.5f, 0.0f, 0.0f)),

		Vertex(Vec3(-1.0f, -1.0f, -1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(-1.0f, -1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(-1.0f, 1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(-1.0f, 1.0f, -1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),

		Vertex(Vec3(-1.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, 1.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f)),
		Vertex(Vec3(-1.0f, 1.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.5f, 0.0f, 0.0f))
	};
	std::vector<uint32_t> indices = {
		3, 2, 1, 1, 0, 3,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};


	Window::WindProc proc;
	proc.title = "WEngine";
	proc.width = 640;
	proc.height = 480;
	m_window = Window::Create(&proc);

	Framebuffer::SetDepthTest(true);
	
	RenderCommand::Init();
	RenderCommand::SetClearColor(Vec4(0.2f, 0.2f, 0.2f, 1.0f));
	RenderCommand::Clear();

	m_world = World::CreateWorld();
	Entity* sphere = World::CreateEntity(m_world);
	Entity* cube = World::CreateEntity(m_world);
	Camera* camera = World::CreateEntity<Camera>(m_world);
	m_world->m_camera = camera;
	camera->Move(Camera::direction::BACKWARD, 3.0f);
	camera->Move(Camera::direction::RIGHT, 1.0f);
	camera->Move(Camera::direction::UP, 3.0f);

	Mesh* Cube = MeshLibrary::Allocate();
	Cube->AddVertices(&vertices[0], vertices.size());
	Cube->AddIndices(&indices[0], indices.size());
	sphere->AddComponent<Mesh>(Sphere);
	cube->AddComponent<Mesh>(Cube);

	Matrix4x4f proj = Matrix4x4f::GetIdentityMatrix();
	PerspectiveProjection(proj, 45.0f, 640.0f / 480.0f, 1.0f, 100.0f);
	Matrix4x4f model = Matrix4x4f::GetIdentityMatrix();
	sphere->AddComponent<Transformer>();
	sphere->GetTransformer()->SetScale(Vec3(0.2f, 0.2f, 0.2f));
	cube->AddComponent<Transformer>();
	cube->GetTransformer()->SetTranslate(Vec3(-1.0f, 0.0f, 1.0f));
	cube->GetTransformer()->SetScale(Vec3(0.4f, 0.4f, 0.4f));


	PointLight* light = World::CreateEntity<PointLight>(m_world);
	light->SetColor(Vec3(0.5f));
	light->SetPos(Vec3(1.0f, 1.0f, -1.0f));
	m_world->AddLight(light);
	
	sphere->AddComponent<Material>();
	cube->AddComponent<Material>();
	Shader* shader = ShaderLibrary::Allocate();
	static_cast<WindowsShader*>(shader)->SetProjection(proj);
	static_cast<WindowsShader*>(shader)->SetView(camera->GetViewMatrix());
	sphere->GetMaterial()->SetShader(shader);
	cube->GetMaterial()->SetShader(shader);

	sphere->SetVisible(true);
	cube->SetVisible(true);

	Renderer* renderer = RendererLibrary::Allocate(Renderer::Primitive::TRIANGLE);
	m_world->SetRenderer(sphere, renderer);
	m_world->SetRenderer(cube, renderer);
}

void Application::Tick()
{
	while (true)
	{
		m_world->Render();
		m_window->OnUpdate();
	}
}