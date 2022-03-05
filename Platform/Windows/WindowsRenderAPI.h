#pragma once
#include "Render/RenderAPI.h"

class WindowsRenderAPI : public RenderAPI
{
public:
	virtual void Init();
	virtual void SetClearColor(Vec4 Color);
	virtual void Clear();
	virtual void SetViewport(const uint32_t& x, const uint32_t& y, const uint32_t& width, const uint32_t& height);
};