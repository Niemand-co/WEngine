#pragma once
#ifndef SR_FRAMEBUFFER_H
#define SR_FRAMEBUFFER_H

#include "Math/Vector.h"
#include <stdint.h>
#include <vector>

typedef uint8_t BUFFER_TYPE;

extern BUFFER_TYPE COLOR_BUFFER;
extern BUFFER_TYPE DEPTH_BUFFER;

class Window;

class Framebuffer
{
public:
	Framebuffer(uint32_t width, uint32_t height);
	~Framebuffer() = default;

	uint32_t GetWidth();
	uint32_t GetHeight();

	void SetDepthTest(bool test);

	void SetBufferType(BUFFER_TYPE type, uint8_t num);

	void WriteToBuffer(uint32_t x, uint32_t y, std::vector<Vec4> buffer);

	void ClearBuffer(BUFFER_TYPE type);

	void RenderToScreen(Window* window);

	static Framebuffer* cur_framebuffer;
	static Vec4 clear_color;
	static void BindFramebuffer(Framebuffer* framebuffer);
	static void SetClearColor(Vec4 color);
private:
	uint32_t m_width;
	uint32_t m_height;
	bool isDepthTest;
	std::vector<std::vector<Vec4>> m_colorbuffers;
	std::vector<Vec4> m_depthbuffer;
};

#endif