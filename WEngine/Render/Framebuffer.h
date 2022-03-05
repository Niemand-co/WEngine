#pragma once
#ifndef SR_FRAMEBUFFER_H
#define SR_FRAMEBUFFER_H

#include "Math/Vector.h"
#include <stdint.h>
#include <vector>

typedef uint8_t BUFFER_TYPE;

extern BUFFER_TYPE COLOR_BUFFER;
extern BUFFER_TYPE DEPTH_BUFFER;

class WindowsWindow;

class Framebuffer
{
public:
	Framebuffer(uint32_t width, uint32_t height);
	~Framebuffer() = default;

	uint32_t GetWidth();
	uint32_t GetHeight();


	void SetBufferType(BUFFER_TYPE type, uint8_t num);

	void WriteToBuffer(uint32_t x, uint32_t y, std::vector<Vec4> buffer);

	void RenderToScreen(WindowsWindow* window);

	static Framebuffer* cur_framebuffer;
	static Vec4 clear_color;
	static void BindFramebuffer(Framebuffer* framebuffer);
	static void SetClearColor(Vec4 color);
	static void ClearBuffer(BUFFER_TYPE type);
	static void SetDepthTest(bool test);
private:
	uint32_t m_width;
	uint32_t m_height;
	std::vector<std::vector<Vec4>> m_colorbuffers;
	std::vector<Vec4> m_depthbuffer;
	static bool isDepthTest;
};

class FramebufferLibrary
{
public:
	struct StackAllocator
	{
		char memory[60000000];
		StackAllocator* next;
	};

	explicit FramebufferLibrary();
	~FramebufferLibrary();

	void Init();
	void Clear();
	
	Framebuffer* Allocate(uint32_t width, uint32_t height);
	uint32_t GetMark();
	
private:
	StackAllocator* m_head;
	uint32_t m_mark;
	uint32_t size;
};

#endif