#pragma once
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

	virtual bool IsKeyPressed(int keycode) override;

	virtual void SetMousePosition(glm::vec2 pos) override;

	virtual glm::vec2 GetMousePosition() override;

	void SetWidth(unsigned int width) { m_width = width; }

	void SetHeight(unsigned int height) { m_height = height; }

private:

	WinProc m_proc;

	GLFWwindow* m_handle;

	unsigned int m_width;

	unsigned int m_height;

	glm::vec2 m_cursorPosition;

};