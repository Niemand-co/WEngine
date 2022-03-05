#pragma once

class Window;

static enum class BUFFER_NUM
{
	SINGLE_BUFFER = 0,
	DOUBLE_BUFFER
};

class RenderContext
{
public:
	virtual void Init() = 0;
	virtual void SetBufferNum(BUFFER_NUM num) = 0;
	virtual void SwapBuffer() = 0;
};