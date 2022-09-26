#include "pch.h"
#include "Platform/GLFWWindow/Public/GLFWWindow.h"
#include "Math/Matrix.h"
#include "Utils/Public/OBJLoader.h"

static void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int bit)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		RE_LOG("Window Closed.");
		glfwWindowShouldClose(window);
		GLFWWindow* win = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
		win->Destroy();
	}
}

static void WindowCloseCallback(GLFWwindow* window)
{
	RE_LOG("Window Closed.");
	GLFWWindow* win = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
	win->Destroy();
}

static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	GLFWWindow* win = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
	win->SetWidth((unsigned int)width);
	win->SetHeight((unsigned int)height);
	win->SetSizeChanged(true);
	RE_LOG("Window Size Changed.");
}

GLFWWindow::GLFWWindow(WinProc* pProc)
{
	m_proc = *pProc;
	m_width = pProc->width;
	m_height = pProc->height;
	m_isClosed = false;
	Init();
}

GLFWWindow::~GLFWWindow()
{
	Destroy();
}

void GLFWWindow::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_handle = glfwCreateWindow(m_proc.width, m_proc.height, m_proc.name, nullptr, nullptr);
	glfwMakeContextCurrent(m_handle);
	glfwSetWindowUserPointer(m_handle, this);
	glfwSetFramebufferSizeCallback(m_handle, FramebufferResizeCallback);
	glfwSetWindowCloseCallback(m_handle, WindowCloseCallback);
	glfwSetKeyCallback(m_handle, WindowKeyCallback);

	GLFWimage icons[1];
	icons[0].pixels = OBJLoader::ImageLoad("assets/chino.png", &icons[0].width, &icons[0].height, 0, 4);
	glfwSetWindowIcon(m_handle, 1, icons);
}

void GLFWWindow::Destroy()
{
	m_isClosed = true;
	glfwDestroyWindow(m_handle);
	glfwTerminate();
}

void GLFWWindow::Update()
{
	m_isSizeChanged = false;
	glfwPollEvents();
}

void* GLFWWindow::GetHandle() const
{
	return m_handle;
}