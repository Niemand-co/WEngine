#pragma once

namespace WEngine 
{
	class Event;
}

struct WinProc
{
public:
	WinProc(const char* name = "WEngine", unsigned int width = 1920u, unsigned int height = 1080u)
	{
		this->name = name;
		this->width = width;
		this->height = height;
	}

public:
	const char *name;
	unsigned int width;
	unsigned int height;
};

class Window
{
public:

	using EventCallbackFunc = std::function<void(WEngine::Event*)>;

	virtual ~Window() = default;

	virtual void Init() = 0;

	virtual void Destroy() = 0;

	virtual void Update() = 0;

	virtual void* GetHandle() const = 0;

	virtual unsigned int GetWidth() const  = 0;

	virtual unsigned int GetHeight() const = 0;

	virtual void SetWidth(unsigned int) = 0;

	virtual void SetHeight(unsigned int) = 0;

	virtual bool IsKeyPressed(int keycode) = 0;

	virtual void SetMousePosition(glm::vec2 pos) = 0;

	virtual glm::vec2 GetMousePosition() = 0;

	void SetEventCallback(EventCallbackFunc func);

	void ExecuteEventCallback(WEngine::Event *pEvent);

	virtual bool GetIsClosed();

	virtual bool GetShouldClose();

	virtual void SetShouldClose(bool shouldClose);

	bool IsSizeChanged() { return m_isSizeChanged; }

	void SetSizeChanged(bool changed) { m_isSizeChanged = changed; }

	static Window* Get();

	static Window* Get(WinProc* proc);

	static Window* cur_window;

protected:

	bool m_shouldClose;

	bool m_isClosed;

	bool m_isSizeChanged;

	EventCallbackFunc m_eventCallbackFunc;

};