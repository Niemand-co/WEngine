#pragma once
#ifndef SR_WINDOW_H
#define SR_WINDOW_H

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
	virtual ~Window() = default;

	virtual void Init() = 0;

	virtual void Destroy() = 0;

	virtual void Update() = 0;

	virtual void* GetHandle() const = 0;

	virtual unsigned int GetWidth() const  = 0;

	virtual unsigned int GetHeight() const = 0;

	virtual void SetWidth(unsigned int) = 0;

	virtual void SetHeight(unsigned int) = 0;

	virtual bool GetIsClosed();

	static Window* Get();

	static Window* Get(WinProc* proc);

	static Window* cur_window;

protected:

	bool m_isClosed;

};

#endif