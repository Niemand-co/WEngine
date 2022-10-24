#include "pch.h"
#include "Platform/GLFWWindow/Public/GLFWWindow.h"
#include "Math/Matrix.h"
#include "Utils/Public/FileLoader.h"
#include "Event/Public/KeyEvent.h"

static void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int bit)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		RE_LOG("Window Closed.");
		glfwWindowShouldClose(window);
		GLFWWindow* win = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
		win->SetShouldClose(true);
	}
}

static void WindowCloseCallback(GLFWwindow* window)
{
	RE_LOG("Window Closed.");
	GLFWWindow* win = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
	win->SetShouldClose(true);
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
	m_shouldClose = false;
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
	ImageData *data = FileLoader::ImageLoad("assets/chino.png");
	icons[0].pixels = data->pData;
	icons[0].width = data->width;
	icons[0].height = data->height;
	glfwSetWindowIcon(m_handle, 1, icons);

	glfwSetKeyCallback(m_handle, [](GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		GLFWWindow *win = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
		switch (action)
		{
		case GLFW_PRESS:
		{
			WEngine::Event *e = new WEngine::KeyPressedEvent(key, scancode);
			win->ExecuteEventCallback(e);
		}
		default:
			break;
		}
	});
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