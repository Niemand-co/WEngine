#pragma once
#include "Render/Public/RenderAPI.h"

template<typename T>
struct Vector4Type;

typedef Vector4Type<float> Vector4;

class WindowsRenderAPI : public RenderAPI
{
public:
	virtual void Init();
	virtual void SetClearColor(Vector4 Color);
	virtual void Clear();
	virtual void SetViewport(const uint32_t& x, const uint32_t& y, const uint32_t& width, const uint32_t& height);
};