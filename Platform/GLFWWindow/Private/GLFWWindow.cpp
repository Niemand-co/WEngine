#include "pch.h"
#include "Platform/GLFWWindow/Public/GLFWWindow.h"
#include "Math/Matrix.h"
#include "Utils/Public/FileLoader.h"
#include "Event/Public/KeyEvent.h"
#include "Event/Public/MouseButtonEvent.h"
#include "Event/Public/WindowEvent.h"

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
		case GLFW_RELEASE:
		{
			WEngine::Event *e = new WEngine::KeyReleasedEvent(key);
			win->ExecuteEventCallback(e);
		}
		default:
			break;
		}
	});

	glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* window, int button, int action, int mods)
	{
		GLFWWindow *win = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
		switch (action)
		{
		case GLFW_PRESS:
		{
			WEngine::Event *e = new WEngine::MouseButtonPressedEvent(button);
			win->ExecuteEventCallback(e);
			break;
		}
		case GLFW_RELEASE:
		{
			WEngine::Event *e = new WEngine::MouseButtonReleasedEvent(button);
			win->ExecuteEventCallback(e);
			break;
		}
		default:
			break;
		}
	});

	glfwSetCursorPosCallback(m_handle, [](GLFWwindow *window, double xpos, double ypos)
	{
		GLFWWindow* win = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
		win->SetMousePosition(glm::vec2(xpos, ypos));
	});

	glfwSetWindowCloseCallback(m_handle, [](GLFWwindow *window)
	{
		GLFWWindow* win = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
		WEngine::Event *e = new WEngine::WindowCloseEvent();
		win->ExecuteEventCallback(e);
	});

	glfwSetFramebufferSizeCallback(m_handle, [](GLFWwindow *window, int width, int height)
	{
		GLFWWindow *win = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
		win->SetWidth(width);
		win->SetHeight(height);
	});
}

void GLFWWindow::Destroy()
{
	m_isClosed = true;
	glfwDestroyWindow(m_handle);
	glfwTerminate();
}

void GLFWWindow::Poll()
{
	glfwPollEvents();
}

void* GLFWWindow::GetHandle() const
{
	return m_handle;
}

bool GLFWWindow::IsKeyPressed(int32 keycode)
{
	return glfwGetKey(m_handle, keycode) == GLFW_PRESS;
}

bool GLFWWindow::IsMouseClicked(int32 mouseButton)
{
	return glfwGetMouseButton(m_handle, mouseButton) == GLFW_PRESS;
}

void GLFWWindow::SetMousePosition(glm::vec2 pos)
{
	m_cursorPosition = pos;
}

glm::vec2 GLFWWindow::GetMousePosition()
{
	return m_cursorPosition;
}

glm::vec2 GLFWWindow::GetWindowPos()
{
	int xPos, yPos;
	glfwGetWindowPos(m_handle, &xPos, &yPos);
	return glm::vec2(xPos, yPos);
}

glm::vec2 GLFWWindow::GetWindowSize()
{
	int width, height;
	glfwGetWindowSize(m_handle, &width, &height);
	return glm::vec2(width, height);
}
