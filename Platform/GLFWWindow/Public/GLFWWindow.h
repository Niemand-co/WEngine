#pragma once
#ifndef WENGINE_GLFWWINDOW_H
#define WENGINE_GLFWWINDOW_H

#include "Utils/Public/Window.h"


struct GLFWwindow;
struct WinProc;

class GLFWWindow : public Window
{
public:
	GLFWWindow(WinProc* pProc);
	virtual ~GLFWWindow();

	virtual void Init();

	virtual void Destroy();

	virtual void Update();

	virtual void* GetHandle() const override;

	virtual unsigned int GetWidth() const override { return m_width; }

	virtual unsigned int GetHeight() const override { return m_height; }

	void SetWidth(unsigned int width) { m_width = width; }

	void SetHeight(unsigned int height) { m_height = height; }

	bool IsSizeChanged() { return m_isSizeChanged; }

	void SetSizeChanged(bool changed) { m_isSizeChanged = changed; }
private:
	WinProc m_proc;
	GLFWwindow* m_handle;
	unsigned int m_width;
	unsigned int m_height;
	bool m_isSizeChanged;
};

#endif