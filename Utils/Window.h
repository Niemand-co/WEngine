#pragma once
#ifndef SR_WINDOW_H
#define SR_WINDOW_H
#include <string>

class Window
{
public:
	struct WindProc
	{
		std::string title;
		uint32_t width;
		uint32_t height;
	};

	virtual ~Window() = default;

	virtual void* GetWindowHandle() const = 0;
	virtual unsigned int GetWidth() const  = 0;
	virtual unsigned int GetHeight() const = 0;

	virtual void OnUpdate() = 0;

	static Window* Create(WindProc* proc);
};

#endif